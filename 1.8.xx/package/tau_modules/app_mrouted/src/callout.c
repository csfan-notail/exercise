/*
 * The mrouted program is covered by the license in the accompanying file
 * named "LICENSE".  Use of the mrouted program represents acceptance of
 * the terms and conditions listed in that file.
 *
 * The mrouted program is COPYRIGHT 1989 by The Board of Trustees of
 * Leland Stanford Junior University.
 */

#include "defs.h"

/* the code below implements a callout queue */
static int id = 0;
static struct timeout_q  *Q = 0; /* pointer to the beginning of timeout queue */

struct timeout_q {
	struct timeout_q *next;		/* next event */
	int	   	 id;
	cfunc_t          func;	  	/* function to call */
	void	   	 *data;		/* func's data */
	time_t	       	 time;		/* time offset to next event*/
};

#ifdef IGMP_DEBUG
static void print_Q(void);
#else
#define	print_Q()
#endif

void callout_init(void)
{
    Q = (struct timeout_q *) 0;
}

void free_all_callouts(void)
{
    struct timeout_q *p;

    while (Q) {
	p = Q;
	Q = Q->next;
	free(p);
    }
}


/*
 * elapsed_time seconds have passed; perform all the events that should
 * happen.
 */
void age_callout_queue(time_t elapsed_time)
{
    struct timeout_q *ptr;
    int i = 0;

    for (ptr = Q; ptr; ptr = Q, i++) {
	if (ptr->time > elapsed_time) {
	    ptr->time -= elapsed_time;
	    return;
	} else {
	    elapsed_time -= ptr->time;
	    Q = Q->next;
	    IF_DEBUG(DEBUG_TIMEOUT) {
		logit(LOG_DEBUG, 0, "about to call timeout %d (#%d)", ptr->id, i);
	    }

	    if (ptr->func)
		ptr->func(ptr->data);
	    free(ptr);
	}
    }
}

/*
 * Return in how many seconds age_callout_queue() would like to be called.
 * Return -1 if there are no events pending.
 */
int timer_nextTimer(void)
{
    if (Q) {
	if (Q->time < 0) {
	    logit(LOG_WARNING, 0, "timer_nextTimer top of queue says %d", Q->time);
	    return 0;
	}

	return Q->time;
    }

    return -1;
}

/*
 * sets the timer
 * @delay:  number of seconds for timeout
 * @action: function to be called on timeout
 * @data:   what to call the timeout function with
 */
int timer_setTimer(time_t delay, cfunc_t action, void *data)
{
    struct     timeout_q  *ptr, *node, *prev;
    int i = 0;

    /* create a node */
    node = (struct timeout_q *)malloc(sizeof(struct timeout_q));
    if (!node) {
	logit(LOG_WARNING, 0, "Malloc failed in timer_setTimer()\n");
	return -1;
    }

    node->func = action;
    node->data = data;
    node->time = delay;
    node->next = 0;
    node->id   = ++id;

    prev = ptr = Q;

    /* insert node in the queue */

    /* if the queue is empty, insert the node and return */
    if (!Q)
	Q = node;
    else {
	/* chase the pointer looking for the right place */
	while (ptr) {

	    if (delay < ptr->time) {
		/* right place */

		node->next = ptr;
		if (ptr == Q)
		    Q = node;
		else
		    prev->next = node;
		ptr->time -= node->time;

		print_Q();
		IF_DEBUG(DEBUG_TIMEOUT) {
		    logit(LOG_DEBUG, 0, "created timeout %d (#%d)", node->id, i);
		}

		return node->id;
	    } else  {
		/* keep moving */

		delay -= ptr->time; node->time = delay;
		prev = ptr;
		ptr = ptr->next;
	    }
	    i++;
	}
	prev->next = node;
    }

    print_Q();
    IF_DEBUG(DEBUG_TIMEOUT) {
	logit(LOG_DEBUG, 0, "created timeout %d (#%d)", node->id, i);
    }

    return node->id;
}

/* returns the time until the timer is scheduled */
int timer_leftTimer(int timer_id)
{
    struct timeout_q *ptr;
    time_t left = 0;

    if (!timer_id)
	return -1;

    for (ptr = Q; ptr; ptr = ptr->next) {
	left += ptr->time;
	if (ptr->id == timer_id)
	    return left;
    }

    return -1;
}

/* clears the associated timer.  Returns 1 if succeeded. */
int timer_clearTimer(int timer_id)
{
    struct timeout_q  *ptr, *prev;
    int i = 0;

    if (!timer_id)
	return 0;

    prev = ptr = Q;

    /*
     * find the right node, delete it. the subsequent node's time
     * gets bumped up
     */

    print_Q();
    while (ptr) {
	if (ptr->id == timer_id) {
	    /* got the right node */

	    /* unlink it from the queue */
	    if (ptr == Q)
		Q = Q->next;
	    else
		prev->next = ptr->next;

	    /* increment next node if any */
	    if (ptr->next != 0)
		(ptr->next)->time += ptr->time;

	    if (ptr->data)
		free(ptr->data);

	    IF_DEBUG(DEBUG_TIMEOUT) {
		logit(LOG_DEBUG, 0, "deleted timer %d (#%d)", ptr->id, i);
	    }
	    free(ptr);

	    print_Q();

	    return 1;
	}
	prev = ptr;
	ptr = ptr->next;
	i++;
    }

    print_Q();
    IF_DEBUG(DEBUG_TIMEOUT) {
	logit(LOG_DEBUG, 0, "failed to delete timer %d (#%d)", timer_id, i);
    }

    return 0;
}

#ifdef IGMP_DEBUG
/*
 * debugging utility
 */
static void print_Q(void)
{
    struct timeout_q  *ptr;

    IF_DEBUG(DEBUG_TIMEOUT) {
	for (ptr = Q; ptr; ptr = ptr->next)
	    logit(LOG_DEBUG, 0, "(%d,%d) ", ptr->id, ptr->time);
    }
}
#endif /* IGMP_DEBUG */

/**
 * Local Variables:
 *  version-control: t
 *  indent-tabs-mode: t
 *  c-file-style: "ellemtel"
 *  c-basic-offset: 4
 * End:
 */
