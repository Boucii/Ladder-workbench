#include "sdb.h"

#define NR_WP 32

extern word_t expr(char *e,bool *success);
typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char expression[65536];
  word_t old_value;
  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
    wp_pool[i].old_value=0;
    wp_pool[i].expression[0]='\0';
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
void new_wp(char *exp,int *status){
  if(free_==NULL){
    printf("no watchpoint available");
    *status=1;
    return ;
  }else{
    WP* temp=free_;
    free_=free_->next; 
    temp->next=head->next;
    head->next=temp;
    *status=0;
    //update expr here
    strcpy(temp->expression,exp);
    bool *success=false;
    int res=expr(exp,success);
    temp->old_value=res;
    return ;
  }
}
int free_wp(int num){
  WP *prev=head;
  WP *found;
  if(head->NO==num){
    head=NULL;
    found=head;
  }else{
  while(prev->next->NO!=num){
    prev=prev->next;
    if(prev->next==NULL){
      return -1;//not found;
    }
  }
  found=prev->next;
  /*delete from head*/
  prev->next=found->next;
  }
  /*add to free*/
  if(free_==NULL){
    free_=found;
  }else{
    found->next=free_->next;
    free_->next=found;
  }
  return 0;//found;
  }

int difftest(){
  WP *cur=head;
  word_t new_value;
  while(cur!=NULL){
    bool * success=false;
    new_value=expr(cur->expression,success);
    if(new_value!=cur->old_value){
      cur->old_value=new_value;
      printf("changed");
      return 1;//changed
    }
  }
  return 0;//not changed
}    
void print_wp(){
  WP *cur=head; 
  if(cur==NULL){
    printf("No Watchpoints\n");
    return;
  }
  while(cur!=NULL){
    printf("watchpoint %d : %s The value is:%ld\n",cur->NO,cur->expression,cur->old_value);
    cur=cur->next;
  
  }

}
