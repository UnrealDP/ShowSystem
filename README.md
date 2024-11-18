# ShowSystem

1. 시간 기준으로 각 기능을 담고 있는 Key가 실행되면서 연출되는 시스템
2. 언리얼의 Sequencer를 상상하면 비슷한 기능이다.
3. 애니메이션 플레이, 파티클 플레이, 카메라 연출 등의 기능이 하나의 Key로 설정이 가능하다.
4. 추가 기능은 Key를 정의하고 기능을 구현하면 툴에서 간단하게 연출 파일을 편집할 수 있다.

<pre>
<h3 id="top"></h3>
ShowSystem/Runtime/
    ├── <a href="#eshowkeytype">EShowKeyType</a>
    ├── ShowBase/
    ├── ShowSequenceAsset/
    ├── ShowSequencer/
    ├── ShowSequencerComponent/
    ├── ShowPlayer/
    ├── ShowSystem/
    └── ShowKeys/
          ├── ShowAnimStatic/
          ├── ShowCamSequence/
          ├── ShowCamShake/
          ├── ShowCascade/
          └── ShowNiagara/
    └── Animation/
          ├── AnimContainer/
          └── ShowAnimInstance/
    └── Misc/
          ├── ShowAnimMontageNotifyHook/
          ├── ShowPerlinNoiseCamShakePattern/
          ├── ShowSequenceCamShakePattern/
          └── ShowWaveOscCamShakePattern/

ShowSystemEditor/
    ├── ShowMaker/
          ├── CameraPathPointCustom/
          ├── ShowSequencerEditorHelper/
          ├── ShowSequencerNotifyHook/
          ├── SShowCamSequenceDetailsWidget/
          ├── SShowKeyBox/
          ├── SShowKeyBoxHandler/
          ├── SShowKeyDetailsWidget/
          ├── SShowKeyDetailsWidget/
          ├── SShowMakerWidget/
          ├── SShowSequencerControllPanel/
          ├── SShowSequencerEditHeader/
          ├── SShowSequencerEditor/
          └── SShowSequencerScrubBoard/
    ├── AnimContainerFactory/
    ├── AssetTypeActions_AnimContainer/
    ├── AssetTypeActions_ShowSequenceAsset/
    ├── GizmoTranslationComponent/
    ├── ShowSequenceAssetCustomization/
    ├── ShowSequenceAssetFactory/
    ├── ShowSequencerEditorToolkit/
    └── ShowSystemEditor/
<pre>
.
.
.
.
.
.
.
.

<div style="display: flex; align-items: center;">
    <h3 style="margin: 0;">EShowKeyType</h3>
    <a href="#top" style="margin-left: 10px;">Top</a>
</div>
EShowKeyType은 Key의 종류를 정의한 Enum 또는 Struct 집합입니다.

<h3 id="showbase">ShowBase</h3>
ShowBase는 ShowSystem의 기본 클래스로, 공통 기능을 제공합니다

