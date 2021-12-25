What is Notepad Master?
==========================
컴퓨터실 장난용 원격조종 툴입니다

친구들끼리만 써주세요

Usage
==========================

Setup
----------------------
![내부망 IP 확인](https://user-images.githubusercontent.com/67177785/147380889-9700623c-9ac5-4767-9301-9587688f5b59.PNG)  
**[그림1] 내부망 IP 확인**

release를 다운받아 NotepadMasterServer.exe를 실행합니다.

빨간색 박스 안의 IP가 현재 컴퓨터의 내부망 IP입니다.

![서버 IP 입력](https://user-images.githubusercontent.com/67177785/147380910-57650a54-d93f-489f-977f-996f178966cf.PNG)  
**[그림2] 서버 IP 입력**

잘 기억해두고 타겟 컴퓨터로 가서 NotepadMasterClient.exe를 실행시켜 줍시다.

IP는 기억해둔 서버의 내부망 IP를 입력하면 됩니다.

![클라이언트 접속 확인](https://user-images.githubusercontent.com/67177785/147380944-25a804e1-46db-47db-b198-843bbd445029.PNG)  
**[그림3] 클라이언트 접속 확인**

접속이 성공적으로 이뤄졌다면 접속자의 정보들을 리스트에 등록합니다.

클라이언트의 번호를 더블클릭해 원격 조종 창을 띄울 수 있습니다.

Keylog
----------------------

![키로깅 확인](https://user-images.githubusercontent.com/67177785/147380959-ee6cda76-1cef-4a92-afe0-3e00936802f0.PNG)  
**[그림4] 키로깅 확인**

조종 타겟이 키보드를 누를 때마다 입력 정보를 전송합니다(키로깅).

키로그를 키가 입력된 프로세스로 구분해 어떤 프로그램에 키가 입력되었는지 알 수 있고

한글 / 영어 구분은 지원하기 않기 때문에 한국어 뷰와 영어 뷰 둘 중 하나를 선택해 입력된 키로그를 해석할 수 있습니다.

Send Message
----------------------

![메시지 전송 예시](https://user-images.githubusercontent.com/67177785/147380964-710f9df3-c83b-443e-82f6-be5771f8c3db.gif)  
**[그림5] 메시지 전송 예시**

메모장을 통해 타겟에게 일방적으로 메시지를 전달합니다.

메시지 입력창 위쪽에서 폰트를 바꿀 수 있습니다.

딜레이 값을 통해 글자 입력 속도를 조절합니다.

글자 크기는 21, 딜레이는 120이 기본 값입니다.

Remote Control
---------------------

![커맨드 입력 예시](https://user-images.githubusercontent.com/67177785/147380986-7ad24c50-83ab-45a5-8fc0-bb31135475b3.gif)  
**[그림6] 커맨드 입력 예시**

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

아이콘(꼭두각시 인형) 출처 : <a href='https://.pngtree.com/so/만화'>만화 png에서 .pngtree.com/</a>