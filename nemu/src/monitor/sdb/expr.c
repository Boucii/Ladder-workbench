#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
extern word_t paddr_read(paddr_t addr, int len);
enum {
  TK_NOTYPE = 256, TK_EQ,TK_NEQ,TK_DECIMAL,TK_DEREF,TK_HEX,TK_REG,TK_AND,TK_OR,

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {"0x[0-9,a-f]+",TK_HEX},
  {" +",TK_NOTYPE},    // spaces
  {"\\$[0-9,a-z]+",TK_REG},  //not so accurate,modify later
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  {"\\-",'-'},
  {"\\*",'*'},
  {"\\/",'/'},
  {"\\(",'('},
  {"\\)",')'},
  {"([1-9]+[0-9]*)|[0]",TK_DECIMAL},
  {"!=",TK_NEQ},
  {"&&",TK_AND},
  {"||",TK_OR},

};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0; //marks the pos in str e
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;
        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        //assert(substr_len>32);
        switch (rules[i].token_type) {
          default: 
		  tokens[nr_token].type=rules[i].token_type;
                  strncpy(tokens[nr_token].str,substr_start,substr_len);
                  tokens[nr_token].str[substr_len]='\0';
		  break;
          case TK_NOTYPE:
	          nr_token--;
		  break;                                                    	  
	} 
    nr_token++;
    break;
    }
    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
      }
    }
  }
  return true;
}

int find_dominant(int p,int q){
   //find dominant operator in tokens[p:q]
   int op_index[32]={}; 
   int j=0;
   int i=p;
   while(i<=q){
     if(tokens[i].type=='('){
        int braces=1;
       while(braces!=0){
         i++;
	 if(tokens[i].type=='('){braces++;}
	 if(tokens[i].type==')'){braces--;}
       }
       continue;
     }
     if(tokens[i].type!='+'&&tokens[i].type!='-'&&tokens[i].type!='*'&&tokens[i].type!='/'\
		     &&tokens[i].type!=TK_AND&&tokens[i].type!=TK_EQ&&tokens[i].type!=TK_NEQ){
       i++;
       continue;
     }else{
	 op_index[j]=i;
	 j++;
     }
     i++;
   }
  for(j--;j>=0;j--){
    if(tokens[op_index[j]].type==TK_AND||tokens[op_index[j]].type==TK_OR){return op_index[j];}
    if(tokens[op_index[j]].type==TK_EQ||tokens[op_index[j]].type==TK_NEQ){return op_index[j];}
    if(tokens[op_index[j]].type=='+'||tokens[op_index[j]].type=='-'){return op_index[j];}
    if(tokens[op_index[j]].type=='*'||tokens[op_index[j]].type=='/'){return op_index[j];}
  }
  assert(0);
  return -1;
 }
bool check_parentheses(int p, int q){
  int pairs=0;
  if(tokens[p].type!='('&&tokens[q].type!=')') return false;
  while(p<=q){
    if(tokens[p].type=='(') {
	    pairs++;
            if(pairs==0) return false;
    } 
    else if(tokens[p].type==')') {
	    pairs--;
   	    if(pairs==0) return false;
    }
  
  }
  return pairs==0;
}

uint32_t eval(int p, int q) {
  if (p > q) {
    /* Bad expression */
	assert(0);
  }
  else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    if(tokens[p].type==TK_DECIMAL)
      return (uint32_t)atoi(tokens[p].str);
    else if(tokens[p].type==TK_HEX){
      unsigned int val;
      sscanf(tokens[p].str+2,"%x",&val);
      return val;
    }
    else if(tokens[p].type==TK_REG){
      bool success;
      unsigned int val=isa_reg_str2val(tokens[p].str,&success);
      return success?val:0;//how to handle no success? todo
    }
    assert(0);
  }
  else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1);
  }
  else {
    unsigned int val1,val2;
    int op = find_dominant(p,q);
    paddr_t addr;
    if(tokens[op].type==TK_DEREF){
       addr=eval(op+1,q);
       return paddr_read(addr,(int)4);
    }
    val1 = eval(p, op - 1);
    val2 = eval(op + 1, q);

    switch (tokens[op].type) {
      case TK_AND: return val1&val2;
      case TK_OR: return val1|val2;
      case TK_EQ: return val1==val2;
      case TK_NEQ: return val1!=val2;
      case '+': return val1 + val2;
      case '-': return val1 - val2;
      case '*': return val1 * val2;
      case '/': return val1 / val2;
      default: assert(0);
    }
  }
}

word_t expr(char *e, bool *success) {
if (!make_token(e)) {
  *success = false;
  return 0;
}
  *success=true;
/* TODO: Implement code to evaluate the expression. */

for (int i = 0; i < nr_token; i ++) {
  if (tokens[i].type == '*' && (i == 0 && tokens[i - 1].type != TK_DECIMAL \
			  && tokens[i-1].type!=TK_HEX&& tokens[i-1].type!=TK_REG&& tokens[i-1].type!=')') ) {
      tokens[i].type = TK_DEREF;
        }
}

return eval(0, nr_token-1);
}
