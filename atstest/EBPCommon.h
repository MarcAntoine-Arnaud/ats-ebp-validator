/*
** Copyright (C) 2014  Cable Television Laboratories, Inc.
** Contact: http://www.cablelabs.com/
 

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __H_EBP_COMMON_AGS6Q76
#define __H_EBP_COMMON_AGS6Q76

#include "ThreadSafeFIFO.h"
#include "log.h"


#define EBP_NUM_PARTITIONS 10  // 0 - 9

typedef struct
{
   int isBoundary;
   int isImplicit;
   uint32_t implicitPID; // only applicable if implicit -- default is video PID

   uint64_t lastImplicitPTS;
   int isLastImplicitPTSValid;

} ebp_boundary_info_t;

typedef struct
{
   uint32_t PID;  // PID within program for this fifo
   int isVideo; // =1 if this fifo carries video info, =0 if audio

   uint64_t lastVideoChunkPTS;
   int lastVideoChunkPTSValid;

   ebp_boundary_info_t* ebpBoundaryInfo;  // contain info on which partitions are boundaries

   int streamPassFail;  // 1 == pass, 0 == fail

   thread_safe_fifo_t *fifo;

} ebp_stream_info_t;


#endif  // __H_EBP_COMMON_AGS6Q76