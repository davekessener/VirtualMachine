/* Main function for architecture :work:hello_world(behavior) */
#include<freehdl/kernel-handle.hh>
int main (int argc, char *argv[]) 

{
  extern handle_info *L4work_E11hello_world_A8behavior_hinfo;
  extern int kernel_main (int, char *[], handle_info*);
  return kernel_main (argc, argv, L4work_E11hello_world_A8behavior_hinfo);
}

/* end of :work:hello_world(behavior) main function */
