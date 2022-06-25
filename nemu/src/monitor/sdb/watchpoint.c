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
    *status=1;
    if(free_==NULL){
    printf("no watchpoint available");
    return ;
  }else{
    WP* temp=free_;
    free_=free_->next; 
    temp->next=NULL;
    if(head==NULL){
      head=temp;
    }else{
      temp->next=head->next;
      head->next=temp;
    }
    //update expr here
    strcpy(temp->expression,exp);
    bool success=false;
    int res=expr(exp,&success);
    temp->old_value=res;
    *status=(success==1?0:1);
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
  int changed=0;
  WP *cur=head;
  word_t new_value;
  while(cur!=NULL){
    bool success=false;
    new_value=expr(cur->expression,&success);
    if(new_value!=cur->old_value){
      if(strcmp(cur->expression,"$pc")==0){
	word_t old_pc=(word_t)cur->old_value;
        printf("watchpoint %d changed %s:\nold val: 0x%lx,new val:0x%lx \n"\
                      ,cur->NO,cur->expression,old_pc,new_value);
      }else{
        printf("watchpoint %d changed %s:\nold val: %ld,new val:%ld \n"\
		      ,cur->NO,cur->expression,cur->old_value,new_value);
      }
      cur->old_value=new_value;
      changed=1;
    }
    cur=cur->next;
  }
  return changed;
}    
void print_wp(){
  WP *cur=head; 
  if(cur==NULL){
    printf("No Watchpoints\n");
    return;
  }
  while(cur!=NULL){
    if(strcmp(cur->expression,"$pc")){
      printf("watchpoint %d : %s The value is:%#lx \n"\
                ,cur->NO,cur->expression,cur->old_value);
    }else{
      printf("watchpoint %d : %s The value is:%ld\n",cur->NO,cur->expression,cur->old_value);
    }
    cur=cur->next;
  
  }

}
