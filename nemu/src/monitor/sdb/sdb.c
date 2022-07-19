#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"

static int is_batch_mode = false;
extern word_t paddr_read(paddr_t addr, int len);
extern uint32_t eval(int p,int q);
extern void print_wp();
extern void new_wp(char *expr,int *status);
extern int free_wp(int num);

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

static int cmd_si(char *args) {
  char *arg=strtok(NULL," ");
  int step=0;
  if(arg==NULL){
    cpu_exec(1);  
  }else{
    sscanf(arg,"%d",&step);
    if(step<1){
      printf("please enter valid n\n");
    }else{
      cpu_exec(step);
    }
  }

  return 0;
}

static int cmd_info(char *args) {

/* extract the first argument */
char *arg = strtok(NULL, " ");
                                                                      
  if (arg == NULL) {
    /* no argument given */
      printf("a arguement must be followed, see help for more info\n");
    }else if(strcmp(arg,"r")==0){
      isa_reg_display();
    }else if(strcmp(arg,"w")==0){
      print_wp();
    }else{
      printf("plase enter the correct argument, see help for more info\n");
    }
  return 0;
}

static int cmd_x(char *args) {
  char *arg = strtok(NULL, " ");
  int len;
  paddr_t address;
  sscanf(arg,"%d",&len);
  arg = strtok(NULL, " ");
  sscanf(arg+2,"%x",&address);
  
  printf("0x%x:",address);
  int i=0;
  for(;i<len;i++){
    printf("%08lx ",paddr_read(address,(int)4));
    address+=4;
  }
  printf("\n");
  return 0;
}

static int cmd_p(char *args) {
  //char expr[65536];
  char *arg = strtok(NULL, "\0");
  
  bool success;
  unsigned int res= expr(arg,&success);
  if(success){
    if(args[0]=='$'){
     printf("0x%x\n",res); //is a register
    }
    printf("%u\n",res);
  }else{
    printf("invalid expression\n");
  }
  return 0;
}

static int cmd_w(char *args) {
  char *arg = strtok(NULL, "\0");
  
  int status;
  new_wp(arg,&status);
  if(status==0){
    printf("watchpoint added\n");
  }else{
    printf("watchpoint addition failed.\n");
  }
  return 0;
}

static int cmd_d(char *args) {
  char *arg = strtok(NULL, " ");
  int num=0;
  sscanf(arg,"%d",&num);
  int res=free_wp(num); 
  
 if(res==1){
   printf("no such watchpoint\n");
 }else if(res==0){
   printf("watchpoint removed\n");
 }
  return 0;
}



static int cmd_help(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  {"si","Execute a single instruction, format: si N",cmd_si},
  {"info","Display registers's status with info r, watch point info with info w",cmd_info},
  {"x","Display the value of EXPR, in the formation of N*4 bytes starting at expr's assress. Format: x N EXPR",cmd_x},
  {"p","Print the value of EXPR, format: p EXPR",cmd_p},
  {"w","Break on the changing of EXPR, format: w EXPR",cmd_w},
  {"d","Delete watch point indexed N, format: d N",cmd_d},

  /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
