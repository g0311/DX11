#include "pch.h"
#include "AssimpTool.h"
#include "Converter.h"

void AssimpTool::Init()
{
	{
		shared_ptr<Converter> converter = make_shared<Converter>();

		//fbx >> Memory
		converter->ReadAssetFile(L"Kachujin/Mesh.fbx");
		
		//Memory >> CustomData (FILE)
		converter->ExportMaterialData(L"Kachujin/Kachujin");
		converter->ExportModelData(L"Kachujin/Kachujin");

		//CustomData >> Memory
		//Ŭ���̾�Ʈ���� ����
	}
}

void AssimpTool::Update()
{

}

void AssimpTool::Render()
{

}
