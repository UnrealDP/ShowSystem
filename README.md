# ShowSystem

1. 시간 기준으로 각 기능을 담고 있는 Key가 실행되면서 연출되는 시스템
2. 언리얼의 Sequencer를 상상하면 비슷한 기능이다.
3. 애니메이션 플레이, 파티클 플레이, 카메라 연출 등의 기능이 하나의 Key로 설정이 가능하다.
4. 추가 기능은 Key를 정의하고 기능을 구현하면 툴에서 간단하게 연출 파일을 편집할 수 있다.
   
### File

- **ShowSystem/Runtime/**
  - [EShowKeyType](#eshowkeytype)
  - [ShowBase](#showbase)
  - ShowSequenceAsset/
  - ShowSequencer/
  - ShowSequencerComponent/
  - ShowPlayer/
  - ShowSystem/
  - ShowKeys/
    - ShowAnimStatic/
    - ShowCamSequence/
    - ShowCamShake/
    - ShowCascade/
    - ShowNiagara/
  - Animation/
    - AnimContainer/
    - ShowAnimInstance/
  - Misc/
    - ShowAnimMontageNotifyHook/
    - ShowPerlinNoiseCamShakePattern/
    - ShowSequenceCamShakePattern/
    - ShowWaveOscCamShakePattern/

- **ShowSystemEditor/**
  - ShowMaker/
    - CameraPathPointCustom/
    - ShowSequencerEditorHelper/
    - ShowSequencerNotifyHook/
    - SShowCamSequenceDetailsWidget/
    - SShowKeyBox/
    - SShowKeyBoxHandler/
    - SShowKeyDetailsWidget/
    - SShowMakerWidget/
    - SShowSequencerControllPanel/
    - SShowSequencerEditHeader/
    - SShowSequencerEditor/
    - SShowSequencerScrubBoard/
  - AnimContainerFactory/
  - AssetTypeActions_AnimContainer/
  - AssetTypeActions_ShowSequenceAsset/
  - GizmoTranslationComponent/
  - ShowSequenceAssetCustomization/
  - ShowSequenceAssetFactory/
  - ShowSequencerEditorToolkit/
  - ShowSystemEditor/
<br/>


### EShowKeyType
EShowKeyType은 Show Key의 종류를 정의한 Enum. 새로운 key를 추가할 경우 필히 enum을 추가해야함.
<br/>[Top](#File)<br/>

### ShowBase
편집된 실제 데이터 FShowKey를 정의한 파일.<br/>
데이터인 FShowKey로 생성한 인스턴스인 UShowBase를 정의한 파일.<br/>
Base struct와 class로 다양한 기능의 Key들은 상속받아서 구현된다.<br/>
<pre>
virtual void Initialize() PURE_VIRTUAL(UShowBase::Initialize, );
   - Key가 인스턴스 생성되면서 최초 호출되는 함수
   
virtual void Dispose() PURE_VIRTUAL(UShowBase::Dispose, );
   - Key가 삭제되면서 호출되는 소멸 함수
   
virtual void Play() PURE_VIRTUAL(UShowBase::Play, );
   - Key가 StartTime이 지나서 플레이 시작될 때 호출되는 함수
   
virtual void Stop() PURE_VIRTUAL(UShowBase::Stop, );
   - Key를 멈출때 호출되는 함수 (이 곳에서 멈춤 처리를 해줘야 한다)
   
virtual void Reset() PURE_VIRTUAL(UShowBase::Reset, );
   - Key를 다시 플레이 하기 위해 초기화로 불리는 함수 (대부분 툴에서 호출되는 함수)
   
virtual void Tick(float ScaleDeltaTime, float SystemDeltaTime, float BasePassedTime) PURE_VIRTUAL(UShowBase::Tick, );
   - ScaleDeltaTime : Show 내부 TimeScale 적용된 시간 변화량
   - SystemDeltaTime : Owner로 부터 호출되는 시스템 시간 변화량
   - BasePassedTime : TimeScale 적용된 현재 Key의 플레이된 지난 총 시간
      
virtual void ApplyTimeScale(float FinalTimeScale) PURE_VIRTUAL(UShowBase::ApplyTimeScale, );
   - 왜부에서 TimeScale을 변화해서 실제 최종 Key에 적용되어야 하는 FinalTimeScale 값으로 호출함 (여기서 실제 시간 변화에 대한 코드 해야함)
</pre>

<br/>[Top](#File)<br/>

<h3 id="showbase">ShowBase</h3>
ShowBase는 ShowSystem의 기본 클래스로, 공통 기능을 제공합니다

