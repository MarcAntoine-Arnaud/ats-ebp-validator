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


#include "EBPCommon.h"

int get2DArrayIndex (int fileIndex, int streamIndex, int numStreams)
{
   // the data in all 2D arrays is arranged in the following order:
   //
   // file_1: elemstream_1, elemstream_2, elemstream_3,...elemenstream_N
   // file_2: elemstream_1, elemstream_2, elemstream_3,...elemenstream_N
   // file_3: elemstream_1, elemstream_2, elemstream_3,...elemenstream_N
   // ...
   // file_M: elemstream_1, elemstream_2, elemstream_3,...elemenstream_N
   //
   // where N = numStreams
   
   return fileIndex * numStreams + streamIndex;
}

