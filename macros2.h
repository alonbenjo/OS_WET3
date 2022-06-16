//
// Created by student on 6/13/22.
//

#ifndef HW3_MACROS_H
#define HW3_MACROS_H

//flags:
#define PRINT_THREAD_FLAG 1
#define FUNC_FLAG 0
#define TEST_FLAG 0
#define SHOW_SMASH_FLAG 0
#define PRINT_PARAM_FLAG 0
#define PRINT_IMPORTANT_FLAG 0

//PRINT_THREAD:
#ifdef PRINT_THREAD_FLAG
#define PRINT_THREAD(thread_entry) 												\
	do{ 																		\
		printf("print thread:\n"); 												\
		printf("thread_index:\t%d\n", (thread_entry).thread_index);				\
		printf("#requests:\t%d\n", (thread_entry).all_requests_handled);		\
		printf("#dynamic:\t%d\n", (thread_entry).dynamic_requests_handled);		\
		printf("#static:\t%d\n", (thread_entry).static_requests_handled);		\
	} while(0);
#else
#define PRINT_THREAD(thread_entry)
#endif

//FUNC_FLAG:
#if FUNC_FLAG
#define FUNC_ENTRY()  \
  std::cout << __PRETTY_FUNCTION__ << " --> " << std::endl;
#define FUNC_EXIT()  \
  std::cout << __PRETTY_FUNCTION__ << " <-- " << std::endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif

//

#endif //HW3_MACROS_H




#if FUNC_FLAG

#define FUNC_ENTRY()  \
  std::cout << __PRETTY_FUNCTION__ << " --> " << std::endl;
#define FUNC_EXIT()  \
  std::cout << __PRETTY_FUNCTION__ << " <-- " << std::endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif


#if SHOW_SMASH_FLAG

#define SHOW_SMASH() \
    std::cout << "SHOW_SMASH" << '\n' << SmallShell::getInstance() << std::endl;
#else
#define SHOW_SMASH()
#endif


#if TEST_FLAG
#define TEST(number) \
  std::cout << "test: " << (number) << std::endl;
#else
#define TEST(number)
#endif


#if PRINT_PARAM_FLAG

#define PRINT_PARAM(param) \
    std::cout << "print param:\t " << #param << " = " << (param) << std::endl;
#else
#define PRINT_PARAM(param)
#endif

#if PRINT_IMPORTANT_FLAG

#define PRINT_IMPORTANT(param) \
    std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n" \
         << "print important:\t " << #param << " = " << (param)  << "\n"\
         << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n" << std::endl;
#else
#define PRINT_IMPORTANT(param)
#endif

#if 0
#define DO_SYS(syscall) do { \
        pid_t this_pid = getpid(); \
        if(this_pid == -1)   \
        {                    \
            perror("smash error: getpid failed");\
        }\
        if((syscall) == -1 && getpid() != SmallShell::getInstance().main_pid) {   \
            perror(( std::string("smash error: ") + std::string(#syscall).substr(0, std::string(#syscall).find('(')) + std::string(" failed")).c_str()); \
           std::cout << "LINE:\t" << __LINE__ << std::endl;                     \
           exit(1);            \
        }                       \
} while(0)
#endif

#define DO_SYS(syscall, name) do {    \
        if((syscall) == -1 ) {   \
            perror((std::string("smash error: ") + std::string(#name) + std::string(" failed")).c_str()); \
            throw SmashException();            \
        }                       \
} while(0)

#define DO_SYS_EXIT(syscall, name) do {    \
        if((syscall) == -1 ) {   \
            perror((std::string("smash error: ") + std::string(#name) + std::string(" failed")).c_str()); \
            exit(1);            \
        }                       \
} while(0)
