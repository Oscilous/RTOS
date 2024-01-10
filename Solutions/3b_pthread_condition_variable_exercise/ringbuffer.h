#define QUEUE_SIZE 4
#define ELEMENT_TYPE void*
#define EVER ;;
//object interface intQueue start

// public:
	extern void initialize_intQueue(void);

	extern void* put_mail(void *a);
	extern void* try_put_mail(void *a);
	extern void* get_mail(void *a);	
	extern void* try_get_mail(void *a);	
//object intQueue end