# CSE4070_OS
2022-02 운영체제 pintOS 과제 모음 입니다.

## [Proj0-2] PintOS 기본 자료구조 및 기능
PintOS는 운영 체제의 핵심 컴포넌트를 구현하는 프로젝트로, 다음과 같은 기본 자료구조와 기능을 구현합니다.


## [Proj1] UserProgram:

Argument Passing: 사용자 프로그램이 실행될 때, 사용자가 입력한 인자(argument)를 공백 단위로 분리하여 스택(stack)에 저장합니다. 또한, 이러한 인자들의 개수와 주소 등 필요한 정보를 스택에 올바르게 저장합니다.
User Memory Access: 스택 포인터(stack pointer)가 null 포인터이거나 커널 가상 메모리 또는 매핑되지 않은 가상 메모리에 접근하는 것을 방지하고, 잘못된 접근이 발생한 경우 exit(-1)을 호출하여 프로그램을 종료합니다.
System Calls: 사용자 프로그램에서 시스템 콜을 호출하면 시스템 콜 핸들러를 통해 이러한 호출을 처리합니다. 시스템 콜 핸들러는 스택에 저장된 값들을 사용하여 사용자가 원하는 작업을 수행합니다.

## [Proj2] UserProgram:

File Descriptor: 파일 접근 및 입출력을 위한 파일 디스크립터를 구현합니다. 이를 통해 여러 파일을 동시에 관리하고 조작할 수 있습니다.
System Calls: close, read, write, seek, tell, remove, create, filesize 등의 시스템 콜을 구현하여 파일 조작이 적절하게 이루어질 수 있도록 합니다.
파일 시스템 동기화: 여러 프로세스가 동시에 동일한 파일에 접근하는 경우 경쟁 조건(race condition)을 방지하기 위해 파일에 대한 락(lock)을 사용하여 동시 접근을 제어하고, 락을 적절하게 해제합니다.

## [Proj3] Threads:

Alarm Clock: timer_sleep() 함수를 개선하여 busy-waiting 방식 대신 효율적인 대기(waiting) 방식을 구현합니다.
Priority Scheduling: 스레드 간 우선 순위를 고려하여 스케줄링을 개선하고, 우선 순위에 따라 실행되도록 합니다. Aging을 통해 스레드가 ready 리스트에 머무르는 시간에 비례하여 우선 순위를 높이는 기능을 구현합니다.
Advanced Scheduler: BSD 스케줄러와 같이 각 우선 순위에 대한 별도의 ready 큐를 사용하는 다단계 피드백 큐 스케줄링을 구현합니다. 우선 순위가 높은 큐부터 스케줄링을 수행합니다.
