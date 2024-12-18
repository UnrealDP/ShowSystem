# ShowSystem

1. 시간 기준으로 각 기능을 담고 있는 Key가 실행되면서 연출되는 시스템
2. 언리얼의 Sequencer를 상상하면 비슷한 기능이다.
3. 애니메이션 플레이, 파티클 플레이, 카메라 연출 등의 기능이 하나의 Key로 설정이 가능하다.
4. 추가 기능은 Key를 정의하고 기능을 구현하면 툴에서 간단하게 연출 파일을 편집할 수 있다.
   
### File

- **ShowSystem/Runtime/**
  - [EShowKeyType](#eshowkeytype)
  - [ShowBase](#showbase)
  - [ShowSequenceAsset](#ShowSequenceAsset)
  - [ShowSequencer](#ShowSequencer)
  - [ShowSequencerComponent](#ShowSequencerComponent)
  - [ShowPlayer](#ShowPlayer)
  - [ShowSystem](#ShowSystem)
  - ShowKeys/
    - [ShowAnimStatic](#ShowAnimStatic)
    - [ShowCamSequence](#ShowCamSequence)
    - [ShowCamShake](#ShowCamShake)
    - [ShowCascade](#ShowCascade)
    - [ShowNiagara](#ShowNiagara)
  - Animation/
    - [AnimContainer](#AnimContainer)
    - [ShowAnimInstance](#ShowAnimInstance)
  - Misc/
    - [ShowCameraShakeBase](#ShowCameraShakeBase)
    - [ShowPerlinNoiseCamShakePattern](#ShowPerlinNoiseCamShakePattern)
    - [ShowSequenceCamShakePattern](#ShowSequenceCamShakePattern)
    - [ShowWaveOscCamShakePattern](#ShowWaveOscCamShakePattern)

- **ShowSystemEditor/**
  - ShowMaker/
    - [CameraPathPointCustom](#CameraPathPointCustom)
    - [ShowSequencerEditorHelper](#ShowSequencerEditorHelper)
    - [ShowSequencerNotifyHook](#ShowSequencerNotifyHook)
    - [SShowCamSequenceDetailsWidget](#SShowCamSequenceDetailsWidget)
    - [SShowKeyBox](#SShowKeyBox)
    - [SShowKeyBoxHandler](#SShowKeyBoxHandler)
    - [SShowKeyDetailsWidget](#SShowKeyDetailsWidget)
    - [SShowMakerWidget](#SShowMakerWidget)
    - [SShowSequencerControllPanel](#SShowSequencerControllPanel)
    - [SShowSequencerEditHeader](#SShowSequencerEditHeader)
    - [SShowSequencerEditor](#SShowSequencerEditor)
    - [SShowSequencerScrubBoard](#SShowSequencerScrubBoard)
  - AnimContainerFactory : UFactory
  - AssetTypeActions_AnimContainer : FAssetTypeActions_Base
  - AssetTypeActions_ShowSequenceAsset : FAssetTypeActions_Base
  - [GizmoTranslationComponent](#GizmoTranslationComponent)
  - [ShowSequenceAssetCustomization](#ShowSequenceAssetCustomization)
  - ShowSequenceAssetFactory : UFactory
  - ShowSequencerEditorToolkit : FAssetEditorToolkit
  - ShowSystemEditor : IModuleInterface
<br/>


#           ShowSystem               #

### EShowKeyType
EShowKeyType은 Show Key의 종류를 정의한 Enum. 새로운 key를 추가할 경우 필히 enum을 추가해야함.
<br/>[Top](#File)<br/>

### ShowBase
편집된 실제 데이터 FShowKey를 정의한 파일.<br/>
데이터인 FShowKey로 생성한 인스턴스인 UShowBase를 정의한 파일.<br/>
Base struct와 class로 다양한 기능의 Key들은 상속받아서 구현된다.<br/>
각 키 기능을 구현하는 곳에서 오류, 리소스 없음 등으로 키 실행이 불가능하면 해당 구문에서 바로 EShowKeyState::ShowKey_End 로 키 종료 처리 해줘야 한다.<br/>
물론 키의 기능이 정상 완료되는 곳에서도 바로 EShowKeyState::ShowKey_End 로 키 종료 처리 해줘야 한다.<br/>
키가 Pause되면 TimeScale을 0으로 해서 ApplyTimeScale 의 파라미터 FinalTimeScale을 0.0f로 호출해주니 구현 필수.<br/>
TODO: (DIPI) Stop은 기능이 필요할지 혹은 Cancel로 할지 아직 미정으로 Show를 활용하는 방안을 논의한 후에 결정해야함. 아마 Cancel로 결정될 것으로 예상.<br/>
<pre>
virtual void Initialize() PURE_VIRTUAL(UShowBase::Initialize, );
   - Key가 인스턴스 생성되면서 최초 호출되는 함수
   
virtual void Dispose() PURE_VIRTUAL(UShowBase::Dispose, );
   - Key가 삭제되면서 호출되는 소멸 함수
   
virtual void Play() PURE_VIRTUAL(UShowBase::Play, );
   - Key가 StartTime이 지나서 플레이 시작될 때 호출되는 함수
      
virtual void Reset() PURE_VIRTUAL(UShowBase::Reset, );
   - Key를 다시 플레이 하기 위해 초기화로 불리는 함수 (대부분 툴에서 호출되는 함수)
   
virtual void Tick(float ScaleDeltaTime, float SystemDeltaTime, float BasePassedTime) PURE_VIRTUAL(UShowBase::Tick, );
   - ScaleDeltaTime : Show 내부 TimeScale 적용된 시간 변화량
   - SystemDeltaTime : Owner로 부터 호출되는 시스템 시간 변화량
   - BasePassedTime : TimeScale 적용된 현재 Key의 플레이된 지난 총 시간
      
virtual void ApplyTimeScale(float FinalTimeScale) PURE_VIRTUAL(UShowBase::ApplyTimeScale, );
   - 왜부에서 TimeScale을 변화해서 실제 최종 Key에 적용되어야 하는 FinalTimeScale 값으로 호출함 (여기서 실제 시간 변화에 대한 코드 해야함)

virtual void Stop() PURE_VIRTUAL(UShowBase::Stop, );
   - Key를 멈출때 호출되는 함수 (이 곳에서 멈춤 처리를 해줘야 한다)
   - TODO: (DIPI) 아직 미확정 (Cancel로 할지 Stop으로 할지 결정 필요)
</pre>

<br/>[Top](#File)<br/>

### ShowSequenceAsset
ShowSequenceAsset 은 TArray<FInstancedStruct> ShowKeys 를 맴버로 들고 있으며 FShowKey를 상속받은 struct를 Array로 가진다.<br/>
Key를 가지고 있는 순수한 데이터 어셋
<br/>[Top](#File)<br/>


### ShowSequencer
ShowSequencer 는 ShowSequenceAsset 데이터로 연출을 하기 위해 생성되는 인스턴스<br/>
연출 소유자인 Owner 설정이 필수다. void Initialize(AActor* InShowOwner, TObjectPtr<UShowSequenceAsset> InShowSequenceAsset); 
<br/>[Top](#File)<br/>

### ShowSequencerComponent
ShowSequencerComponent 는 ShowSequencer 를 관리하는 Component
<br/>[Top](#File)<br/>

### ShowPlayer
ShowPlayer 는 ShowSequencer 를 플레이하는 인터페이스.
<br/>[Top](#File)<br/>

### ShowSystem
ShowSystem 은 ShowSystem을 구동하는데 필요한 기본 정의와 툴에서 필요한 기본 기능을 정의하는 클래스<br/>
새로운 키를 추가하게 되면 필히 이곳에 추가된 키에 대한 코딩이 필요하다.
<br/>[Top](#File)<br/>

### ShowAnimStatic
사용자의 필요성에 따라 확장이 필요<br/>
지금은 테스트 용으로 간단하게 몽타주 플레이만 구현<br/>
<br/>[Top](#File)<br/>

### ShowCamSequence
ShowCamSequence 카메라를 특정 포인트들의 리스트로 이동사켜주는 카메라 연출 기능.<br/>
FInterpCurve<FVector> 의 커브 기능을 이용해서 카메라 연출을 한다.<br/>
TODO: (DIPI) 자세한 기능 설명은 툴 사용법 문서 작성하고 참고를 걸겠음.<br/>
<br/>[Top](#File)<br/>

### ShowCamShake
ShowCamShake는 언리얼에서 기본적으로 제공하는 3가지 연출 모드를 지원함.<br/>
UPerlinNoiseCameraShakePattern
UWaveOscillatorCameraShakePattern
USimpleCameraShakePattern
UCompositeCameraShakePattern -> 미구현, 필요하면 그때 추가
<br/>[Top](#File)<br/>

### ShowCascade
ShowCascade는 기본적으로 Socket Attach<br/>
모든 옵션을 World로 하면 Attach는 하지 않음<br/>
기본 Attach 옵션을 그대로 따라가지만 KeepWorldThenFollowRoot 옵션 추가됨.<br/>
- KeepRelative : 부모의 값에 상대적으로 따라감
- KeepWorld : 부모 무시 월드
- SnapToTarget : 부모의 값을 그대로 사용함
- KeepWorldThenFollowRoot : 어태치 될때 부모의 값을 적용 후 부모의 값을 더이상 적용 안하고 대신 붙은 후에는 root의 값을 적용함
<br/>[Top](#File)<br/>

### ShowNiagara
TODO: (DIPI) ShowNiagara 는 아직 미구현 (ShowCascade와 동작 법은 똑같으니 그대로 만들면 됨)
<br/>[Top](#File)<br/>

### AnimContainer
애니메이션 정의 데이터<br/>
임시 테스트용. 애니메이션 시스템에 맞춰 삭제 혹은 변경해야 한다.
<br/>[Top](#File)<br/>

### ShowAnimInstance
애니메이션 관리하는 클래스 UAnimInstance 상속<br/>
임시 테스트용. 애니메이션 시스템에 맞춰 삭제 혹은 변경해야 한다.
<br/>[Top](#File)<br/>

### ShowCameraShakeBase
특별한 기능은 없고 카메라 쉐이크를 하기 위한 Base Class. UCameraShakeBase 상속함.
<br/>[Top](#File)<br/>

### ShowPerlinNoiseCamShakePattern
언리얼 지원 카메레 쉐이크인 UPerlinNoiseCameraShakePattern 상속받아 Show용으로 만든 Calss.
<br/>[Top](#File)<br/>

### ShowSequenceCamShakePattern
언리얼 지원 카메레 쉐이크인 UCameraShakePattern 상속받아 Show용으로 만든 Calss.
<br/>[Top](#File)<br/>

### ShowWaveOscCamShakePattern
언리얼 지원 카메레 쉐이크인 UWaveOscillatorCameraShakePattern 상속받아 Show용으로 만든 Calss.
<br/>[Top](#File)<br/>

#           ShowSystemEditor               #

### CameraPathPointCustom
IPropertyTypeCustomization 상속받아서 FShowCamSequenceKey의 편집 Slate를 커스텀한 Class
<br/>[Top](#File)<br/>

### ShowSequencerEditorHelper
UShowSequencer 를 맴버로 가지고 있으며, 에디터에서 UShowSequenceAsset 데이터를 편집하기 위한 Helper Class.<br/>
Add Key, Remove Key 등 현재 선택된 UShowSequenceAsset 의 데이터를 조작하는 용도의 Class.
<br/>[Top](#File)<br/>

### ShowSequencerNotifyHook
Detail View Window 에서 UShowSequenceAsset 의 데이터 변화를 감지하기 위한 Noti Class.
<br/>[Top](#File)<br/>

### SShowCamSequenceDetailsWidget
[Widget] UShowCamSequence 의 Detail View Window 에 카메라의 프리뷰 등 기능을 추가한 커스텀 Class.
<br/>[Top](#File)<br/>

### SShowKeyBox
[Widget] ShowMaker 툴에서 Show Key를 비쥬얼라이즈한 박스.
<br/>[Top](#File)<br/>

### SShowKeyBoxHandler
[Widget] UShowSequenceAsset 에 있는 여러 Key를 SShowKeyBox 로 그려주는 Class.
<br/>[Top](#File)<br/>

### SShowKeyDetailsWidget
[Widget] 선택된 Show Key의 상세 정보를 보여주는 위젯.
<br/>[Top](#File)<br/>

### SShowMakerWidget
[Widget] UShowSequenceAsset 을 더블클릭하면 뜨는 위젯 / UShowSequenceAsset 의 플레이 확인 및 편집하는 최상위 위젯.
<br/>[Top](#File)<br/>

### SShowSequencerControllPanel
[Widget] UShowSequencer 의 동작을 모은 위젯. Play, Pause, TimeScale 등.
<br/>[Top](#File)<br/>

### SShowSequencerEditHeader
[Widget] Show Key의 정보를 보여주는 좌측에 표시되는 header. Key Add 버튼을 가지고 있음.
<br/>[Top](#File)<br/>

### SShowSequencerEditor
[Widget] UShowSequenceAsset 의 기본 편집 위젯을 모아둔 Class.
<br/>[Top](#File)<br/>

### SShowSequencerScrubBoard
[Widget] Show Key들의 플레이를 보여주는 Board를 그리는 위젯.
<br/>[Top](#File)<br/>

### GizmoTranslationComponent
X, Y, Z 축으로 이동 표시를 지원하는 컴포넌트.
<br/>[Top](#File)<br/>

### ShowSequenceAssetCustomization
IDetailCustomization 상속받아서 ShowSequenceAsset 의 Detail View 커스텀.
<br/>[Top](#File)<br/>


