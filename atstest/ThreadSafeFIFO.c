/*
Copyright (c) 2015, Cable Television Laboratories, Inc.(“CableLabs”)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of CableLabs nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL CABLELABS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "ThreadSafeFIFO.h"
#include "EBPThreadLogging.h"


int fifo_create (thread_safe_fifo_t *fifo, int id)
{
   fifo->id = id;

   int returnCode = pthread_mutex_init(&(fifo->fifo_mutex), NULL);
   if (returnCode != 0)
   {
      printThreadDebugMessage ("fifo_create (%d): Error %d calling pthread_mutex_init\n", fifo->id, returnCode);
      return -1;
   }

   returnCode = pthread_cond_init(&(fifo->fifo_nonempty_cond), NULL);
   if (returnCode != 0)
   {
      printThreadDebugMessage ("fifo_create (%d): Error %d calling pthread_cond_init\n", fifo->id, returnCode);
      return -1;
   }

   fifo->queue = varray_new();

   return 0;
}

int fifo_destroy (thread_safe_fifo_t *fifo)
{
   int returnCode = pthread_mutex_destroy(&(fifo->fifo_mutex));
   if (returnCode != 0)
   {
      printThreadDebugMessage ("fifo_destroy (%d): Error %d calling pthread_mutex_destroy\n", fifo->id, returnCode);
   }

   returnCode = pthread_cond_destroy(&(fifo->fifo_nonempty_cond));
   if (returnCode != 0)
   {
      printThreadDebugMessage ("fifo_destroy (%d): Error %d calling pthread_cond_destroy\n", fifo->id, returnCode);
   }

   varray_free(fifo->queue);

   return 0;
}

int fifo_push (thread_safe_fifo_t *fifo, void *element)
{
   int returnCode = 0;
   printThreadDebugMessage ("fifo_push (%d): entering...\n", fifo->id);

   printThreadDebugMessage ("fifo_push (%d): calling pthread_mutex_lock\n", fifo->id);
   returnCode = pthread_mutex_lock (&(fifo->fifo_mutex));
   if (returnCode != 0)
   {
      printThreadDebugMessage ("fifo_push (%d): error %d calling pthread_mutex_lock\n", fifo->id, returnCode);
      return -1;
   }


   printThreadDebugMessage ("fifo_push (%d): doing push\n", fifo->id);

   // do push onto queue here
   varray_insert(fifo->queue, 0, element);

   fifo->push_counter++;
   
   printThreadDebugMessage ("fifo_push (%d): doing push: fifo->value = %p\n", fifo->id, element);


   printThreadDebugMessage ("fifo_push (%d): calling pthread_cond_signal\n", fifo->id);
   returnCode = pthread_cond_signal(&(fifo->fifo_nonempty_cond));
   if (returnCode != 0)
   {
      printThreadDebugMessage ("fifo_push (%d): error %d calling pthread_cond_signal\n", fifo->id, returnCode);
      // unlock mutex before returning
      pthread_mutex_unlock (&(fifo->fifo_mutex));
      return -1;  
   }

   printThreadDebugMessage ("fifo_push (%d): calling pthread_mutex_unlock\n", fifo->id);
   returnCode = pthread_mutex_unlock (&(fifo->fifo_mutex));
   if (returnCode != 0)
   {
      printThreadDebugMessage ("fifo_push (%d): error %d calling pthread_mutex_unlock\n", fifo->id, returnCode);
      return -1;
   }

   printThreadDebugMessage ("fifo_push (%d): exiting\n", fifo->id);
   return 0;
}

int fifo_pop (thread_safe_fifo_t *fifo, void **element)
{
   return fifo_pop_peek (fifo, element, 1 /* isPop */);
}

int fifo_peek (thread_safe_fifo_t *fifo, void **element)
{
   return fifo_pop_peek (fifo, element, 0 /* isPop */);
}

int fifo_pop_peek (thread_safe_fifo_t *fifo, void **element, int isPop)
{
   int returnCode = 0;

   printThreadDebugMessage ("fifo_pop_peek (%d): entering...\n", fifo->id);

   returnCode = pthread_mutex_lock (&(fifo->fifo_mutex));
   if (returnCode != 0)
   {
      printThreadDebugMessage ("fifo_pop_peek (%d): error %d calling pthread_mutex_lock\n", fifo->id, returnCode);
      return -1;
   }

   printThreadDebugMessage ("fifo_pop_peek (%d): doing pop\n", fifo->id);

   // check queue not empty here
   if (varray_length(fifo->queue) != 0)
   {
      printThreadDebugMessage ("fifo_pop_peek (%d): queue not empty -- fifo_pop setting element\n", fifo->id);
   }
   else
   {
      printThreadDebugMessage ("fifo_pop_peek (%d): queue empty -- pop entering wait\n", fifo->id);
      returnCode = pthread_cond_wait(&(fifo->fifo_nonempty_cond), &(fifo->fifo_mutex));
      if (returnCode != 0)
      {
         printThreadDebugMessage ("fifo_pop_peek (%d): error %d calling pthread_cond_signal\n", fifo->id, returnCode);
         // unlock mutex before returning
         pthread_mutex_unlock (&(fifo->fifo_mutex));
         return -1;
      }

   }

   if (isPop)
   {
      printThreadDebugMessage ("fifo_pop_peek (%d): calling varray_pop\n", fifo->id);
      *element = varray_pop(fifo->queue);
      fifo->pop_counter++;
   }
   else
   {
      printThreadDebugMessage ("fifo_pop_peek (%d): calling varray_peek\n", fifo->id);
      *element = varray_peek(fifo->queue);
   }

   printThreadDebugMessage ("fifo_pop_peek (%d): setting element: *element = %p\n", fifo->id, *element);

   printThreadDebugMessage ("fifo_pop_peek (%d): calling pthread_mutex_unlock\n", fifo->id);
   returnCode = pthread_mutex_unlock (&(fifo->fifo_mutex));
   if (returnCode != 0)
   {
      printThreadDebugMessage ("fifo_pop_peek (%d): Error %d calling pthread_mutex_unlock\n", fifo->id, returnCode);
      return -1;
   }

   printThreadDebugMessage ("fifo_pop_peek (%d): exiting\n", fifo->id);
   return 0;
}

int fifo_get_state (thread_safe_fifo_t *fifo, int *size)
{
   int returnCode = 0;
   printThreadDebugMessage ("fifo_get_state (%d): entering\n", fifo->id);

   returnCode = pthread_mutex_lock (&(fifo->fifo_mutex));
   if (returnCode != 0)
   {
      printThreadDebugMessage ("fifo_get_state (%d): Error %d calling pthread_mutex_lock\n", fifo->id, returnCode);
      return -1;
   }

   *size = varray_length(fifo->queue);

   returnCode = pthread_mutex_unlock (&(fifo->fifo_mutex));
   if (returnCode != 0)
   {
      printThreadDebugMessage ("fifo_get_state (%d): Error %d calling pthread_mutex_unlock\n", fifo->id, returnCode);
      return -1;
   }

   return 0;
}

