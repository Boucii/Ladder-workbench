#include <common.h>
#include <syscall.h>

void do_syscall(Context *c);

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case EVENT_YIELD: printf("Oh MAN!!It IS B??\n");break;
    case EVENT_SYSCALL: do_syscall(c); break;
	case EVENT_ERROR :{  break;}
    default: panic("Unhandled event ID = %d", e.event);

  }
 //   asm("csrr a1,mepc");
 //   asm("addi a1,a1,4");
 //   asm("csrw mepc,a1");

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
