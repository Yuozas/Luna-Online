07/10/22 이웅주


프로세스:

1. DB서버가 위치한 곳마다 RM 서버를 실행시킨다.
2. 클라이언트는 모든 곳의 RM서버에 접속한다. RM툴 접속을 위한 로그인 정보는 TB_RECOVERY_MANAGER 에 있어야 한다
	그렇지 않으면 Agent 같은 집중화된 관리 서버가 필요하나, 일정상 어렵다.
3. 클라이언트는 각 서버에 모두 로그인한다. 서버 정보는 클라이언트에 스크립트 파일로 저장되어 있다. 로그인에 실패한 서버가 있으면 알려준다.


그리드 컨트롤 팁
1. 셀의 READ ONLY 처리
	CGridCtrl::SetItem( GV_ITEM* ) 에서 GV_ITEM.nState는 제대로 적용되지 않음을 유의. 
	값을 세팅한 후 CGridCtrl::SetItemState()를 이용하여 조작해야한다.
	
	
07/12/10 이웅주

GIF 이미지를 바꾸려면
	파일명을 BIN등으로 바꾸고 비쥬얼 스튜디오에 드래그하면 바이너리 내용을 복사할 수 있다.
	전체를 복사한 후 해당 리소스를 열고 복사해넣으면 된다.
	

08/01/21

리소스 지역화

client.rc를 기준으로 작업하자. 영문판은 이를 바탕으로 client_english.rc 파일이 있다.
새로운 리소스가 추가되면 잊지말고, WinMerge(이 도구가 가장 변경 내역을 파악하기 편하다)를 
사용하여 추가된 리소스를 업데이트해준다. 디버그 버전은 지역화를 하지 말아야한다.
디버깅을 하려면 파일을 오른쪽 클릭해서 속성을 선택한 다음, 빌드에서 제외를 선택하여
디버깅하려는 리소스를 '아니오'로 선택하고, 그렇지 않은 리소스는 '예'를 선택한다

유니코드를 적용하면 좋으나, 내부 모듈이 그렇게 지원하지 않는다.