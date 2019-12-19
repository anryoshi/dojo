nanotest
========

Small testing helper that you can always bring with yourself

Usage example:
--------------

```c
/* Define test function that returns nanotest_error */ 
struct nanotest_error your_test_function() {
	/* ... */
	/* Perform assertion with nanotest_assert */
	nanotest_assert(your_check(), "your_message_for_failure");
	/* ... */
	/* Use nanotest_success to return empty error from function */
	nanotest_success();
}

/* ... */
/* Call test function with nanotest_run macro */
nanotest_run(your_test_function);
```