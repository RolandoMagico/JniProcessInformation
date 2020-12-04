/***********************************************************************************************************************
 MIT License

 Copyright(c) 2020 Roland Reinl

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files(the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions :

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
***********************************************************************************************************************/

/***********************************************************************************************************************
 INCLUDES
***********************************************************************************************************************/
#include "rolandomagico_processinformation_ProcessInformation.h"
#include <windows.h>
#include <tlhelp32.h>

/***********************************************************************************************************************
 DEFINES
***********************************************************************************************************************/

/***********************************************************************************************************************
 TYPES
***********************************************************************************************************************/

/***********************************************************************************************************************
 LOCAL CONSTANTS
***********************************************************************************************************************/

/***********************************************************************************************************************
 LOCAL VARIABLES
***********************************************************************************************************************/

/***********************************************************************************************************************
 LOCAL FUNCTION DECLARATIONS
***********************************************************************************************************************/

/***********************************************************************************************************************
 IMPLEMENTATION
***********************************************************************************************************************/
JNIEXPORT void JNICALL Java_rolandomagico_processinformation_ProcessInformation_getAllProcesses(JNIEnv* env, jclass currentClass, jobject targetList)
{
  if (targetList == nullptr)
  {
    // No target list instance, unable to continue
  }
  else
  {
    jclass processDataClass = env->FindClass("rolandomagico/processinformation/ProcessData");
    jclass processDataListClass = env->FindClass("java/util/List");

    if ((processDataClass != nullptr) && (processDataListClass != nullptr))
    {
      jmethodID constructor = env->GetMethodID(processDataClass, "<init>", "()V");
      jfieldID processIdField = env->GetFieldID(processDataClass, "processId", "J");
      jfieldID parentProcessIdField = env->GetFieldID(processDataClass, "parantProcessId", "J");
      jfieldID processNameField = env->GetFieldID(processDataClass, "name", "Ljava/lang/String;");
      jmethodID addToListMethod = env->GetMethodID(processDataListClass, "add", "(Ljava/lang/Object;)Z");

      if ((constructor != nullptr) && (processIdField != nullptr) && (processNameField != nullptr))
      {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot != INVALID_HANDLE_VALUE)
        {
          PROCESSENTRY32 processEntry = { 0 };
          processEntry.dwSize = sizeof(PROCESSENTRY32);

          while (Process32Next(snapshot, &processEntry) == TRUE)
          {
            jobject processData = env->NewObject(processDataClass, constructor);
            env->SetLongField(processData, processIdField, processEntry.th32ProcessID);
            env->SetLongField(processData, parentProcessIdField, processEntry.th32ParentProcessID);
            jstring javaString = env->NewString((jchar*)processEntry.szExeFile, (jsize)wcslen(processEntry.szExeFile));
            env->SetObjectField(processData, processNameField, javaString);
            env->CallBooleanMethod(targetList, addToListMethod, processData);
          }
        }
      }
    }
  }
}

JNIEXPORT jlong JNICALL Java_rolandomagico_processinformation_ProcessInformation_getCurrentProcessId(JNIEnv* env, jclass currentClass)
{
  return GetCurrentProcessId();
}

/***********************************************************************************************************************
 EOF
***********************************************************************************************************************/