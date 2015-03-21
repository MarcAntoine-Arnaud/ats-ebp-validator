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


#ifndef __H_ATS_TEST_APP_CONFIG
#define __H_ATS_TEST_APP_CONFIG

typedef struct 
{
   int logLevel;
   char *logFilePath;
   unsigned int ebpPrereadSearchTimeMsecs;
//   unsigned int ebpAllowedPTSJitterSecs;
//   unsigned int ebpSCTE35PTSJitterSecs;
   float ebpAllowedPTSJitterSecs;
   float ebpSCTE35PTSJitterSecs;

   int socketRcvBufferSz;
   int ingestCircularBufferSz;

} ats_test_app_config_t;


extern ats_test_app_config_t g_ATSTestAppConfig;

int readTestConfigFile();
void printTestConfig();
void setTestConfigDefaults();
int initTestConfig();

char *trimWhitespace(char *string);



#endif // __H_ATS_TEST_APP_CONFIG