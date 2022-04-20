# C3DDemo
PhysX를 사용한 DPhysics로 오브젝트간 물리 처리를 Direct3D 11로 기본적인 렌더링을 하는 C3DEngine을 통해 보여줍니다
현재 데모(C3DDemo)에서는 박스 1000개를 생성하여 충돌을 보여줍니다
가운데 생성된 상자는 WASD 키로 이동이 가능합니다

향후 DPhysics로 구현된 기능(캐릭터 컨트롤러, 충돌 콜백, 충돌 필터 등)을 보여줄 수 있는 데모를 추가하고
그에 따른 렌더러 기능 추가를 할 예정입니다

FBX 모델 기능(작업중)
FBX SDK는 용량이 큰 관계로 포함시키지 않았습니다
FBX SDK 2020.3.1 VS2019 를 다운받아 설치한 뒤 설치 위치에서 include, lib 폴더를 FBX 폴더에 그대로 추가하면 빌드 할 수 있습니다
https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2020-3
