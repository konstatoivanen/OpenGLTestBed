#include "PrecompiledHeader.h"
#include "DebugEngine.h"
#include "Context/Structs/Implementers.h"
#include "Context/Structs/EntityViews.h"
#include "Rendering/MeshUtility.h"
#include "Core/Application.h"

static void CreateMeshRenderable(PKECS::EntityDatabase* entityDb, const float3& position, const float3& rotation, Ref<Mesh> mesh, Weak<Material> material)
{
	auto egid = PKECS::EGID(entityDb->ReserveEntityId(), (uint)PKECS::ENTITY_GROUPS::ACTIVE);
	auto implementer = entityDb->ResereveImplementer<PKECS::Implementers::MeshRenderableImplementer>();
	auto baseView = entityDb->ReserveEntityView<PKECS::EntityViews::BaseRenderable>(egid);
	auto meshView = entityDb->ReserveEntityView<PKECS::EntityViews::MeshRenderable>(egid);

	baseView->bounds = static_cast<PKECS::Components::Bounds*>(implementer);
	baseView->handle = static_cast<PKECS::Components::RenderableHandle*>(implementer);
	meshView->materials = static_cast<PKECS::Components::Materials*>(implementer);
	meshView->mesh = static_cast<PKECS::Components::MeshReference*>(implementer);
	meshView->transform = static_cast<PKECS::Components::Transform*>(implementer);

	implementer->aabb = CGMath::CreateBoundsCenterExtents(position, CG_FLOAT3_ONE);
	implementer->isCullable = true;
	implementer->isVisible = false;
	implementer->position = position;
	implementer->rotation = glm::quat(rotation);
	implementer->scale = CG_FLOAT3_ONE;
	implementer->sharedMaterials.push_back(material);
	implementer->sharedMesh = mesh;
	implementer->type = PKECS::Components::RenderHandleType::MeshRenderer;
	implementer->viewSize = 1.0f;
}

static void CreatePointLight(PKECS::EntityDatabase* entityDb, const float3& position, const color& color, float radius)
{
	auto egid = PKECS::EGID(entityDb->ReserveEntityId(), (uint)PKECS::ENTITY_GROUPS::ACTIVE);
	auto implementer = entityDb->ResereveImplementer<PKECS::Implementers::PointLightImplementer>();
	auto baseView = entityDb->ReserveEntityView<PKECS::EntityViews::BaseRenderable>(egid);
	auto lightView = entityDb->ReserveEntityView<PKECS::EntityViews::PointLightRenderable>(egid);

	baseView->bounds = static_cast<PKECS::Components::Bounds*>(implementer);
	baseView->handle = static_cast<PKECS::Components::RenderableHandle*>(implementer);
	lightView->pointLight = static_cast<PKECS::Components::PointLight*>(implementer);
	lightView->transform = static_cast<PKECS::Components::Transform*>(implementer);

	implementer->aabb = CGMath::CreateBoundsCenterExtents(position, CG_FLOAT3_ONE * 0.5f * radius);
	implementer->isCullable = true;
	implementer->isVisible = false;
	implementer->position = position;
	implementer->color = color;
	implementer->radius = radius;
	implementer->type = PKECS::Components::RenderHandleType::PointLight;
	implementer->viewSize = 1.0f;
}

DebugEngine::DebugEngine(AssetDatabase* assetDatabase, Time* time, PKECS::EntityDatabase* entityDb)
{
	m_entityDb = entityDb;
	m_assetDatabase = assetDatabase;
	m_time = time;

	//meshCube = MeshUtilities::GetBox(CG_FLOAT3_ZERO, { 10.0f, 0.5f, 10.0f });
	meshSphere = MeshUtilities::GetSphere(CG_FLOAT3_ZERO, 1.0f);
	materialMetal = assetDatabase->Find<Material>("M_Metal_Panel");
	materialGravel = assetDatabase->Find<Material>("M_Gravel");
	cornellBoxMaterial = assetDatabase->Find<Material>("M_Metal_Panel1");
	cornellBox = assetDatabase->Find<Mesh>("cornell_box");

	auto minpos = float3(-5, -5, -5);
	auto maxpos = float3(5, 5, 5);

	CreateMeshRenderable(entityDb, CGMath::RandomRangeFloat3(minpos, maxpos), { 0,0,0 }, meshSphere, materialMetal);
	CreateMeshRenderable(entityDb, CGMath::RandomRangeFloat3(minpos, maxpos), { 0,0,0 }, meshSphere, materialMetal);
	CreateMeshRenderable(entityDb, CGMath::RandomRangeFloat3(minpos, maxpos), { 0,0,0 }, meshSphere, materialGravel);

	CreatePointLight(entityDb, CGMath::RandomRangeFloat3(minpos, maxpos), CGMath::HueToRGB(CGMath::RandomFloat()) * CGMath::RandomRangeFloat(0.5f, 4.0f), CGMath::RandomRangeFloat(5, 40));
	CreatePointLight(entityDb, CGMath::RandomRangeFloat3(minpos, maxpos), CGMath::HueToRGB(CGMath::RandomFloat()) * CGMath::RandomRangeFloat(0.5f, 4.0f), CGMath::RandomRangeFloat(5, 40));
	CreatePointLight(entityDb, CGMath::RandomRangeFloat3(minpos, maxpos), CGMath::HueToRGB(CGMath::RandomFloat()) * CGMath::RandomRangeFloat(0.5f, 4.0f), CGMath::RandomRangeFloat(5, 40));
	CreatePointLight(entityDb, CGMath::RandomRangeFloat3(minpos, maxpos), CGMath::HueToRGB(CGMath::RandomFloat()) * CGMath::RandomRangeFloat(0.5f, 4.0f), CGMath::RandomRangeFloat(5, 40));

	//instanceMatrices = CreateRef<ComputeBuffer>(BufferLayout({ { CG_TYPE_FLOAT4X4, "Matrix" } }), 2);
	//
	//float4x4 matrices[] = { CG_FLOAT4X4_IDENTITY , CGMath::GetMatrixTRS({ 5.0f, 0.0f, 0.0f }, glm::quat({1,2,3}), CG_FLOAT3_ONE) };
	//instanceMatrices->MapBuffer(matrices, CG_TYPE_SIZE_FLOAT4X4 * 2);
}

DebugEngine::~DebugEngine()
{
	meshSphere = nullptr;
}

void DebugEngine::Step(Input* input)
{
	if (input->GetKeyDown(KeyCode::ESCAPE))
	{
		Application::Get().Close();
		return;
	}

	if (input->GetKeyDown(KeyCode::T))
	{
		materialMetal.lock()->GetShader().lock()->ListProperties();
	}

	if (input->GetKeyDown(KeyCode::R))
	{
		auto shader = materialMetal.lock()->GetShader();
		m_assetDatabase->Reload<Shader>(shader);
		PK_CORE_LOG("Reimported shader: %s", shader.lock()->GetFileName().c_str());
	}

	if (input->GetKey(KeyCode::C))
	{
		m_time->Reset();
	}

	m_time->LogFrameRate();
}

void DebugEngine::Step(int condition)
{
	//Graphics::SetRenderTarget(renderTarget);
	//Graphics::Clear(CG_COLOR_CLEAR, 1.0f, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
	LightingUtility::SetOEMTextures(reflectionMap.lock()->GetGraphicsID(), 1);
	
	Graphics::SetGlobalComputeBuffer(hashCache->pk_InstancingData, instanceMatrices->GetGraphicsID());

	Graphics::Blit(iblShader.lock());

	Graphics::SetGlobalKeyword(hashCache->PK_ENABLE_INSTANCING, true);
	Graphics::DrawMeshInstanced(meshSphere, 0, 2, materialMetal.lock());
	Graphics::SetGlobalKeyword(hashCache->PK_ENABLE_INSTANCING, false);

	Graphics::DrawMesh(meshSphere, 0, materialGravel.lock(), CGMath::GetMatrixTRS({-5.0f, 0.0f, 0.0f}, CG_QUATERNION_IDENTITY, CG_FLOAT3_ONE));

	auto count = cornellBox.lock()->GetSubmeshCount();
	auto matrix = CGMath::GetMatrixTRS({ 10.0f, 0.0f, 0.0f }, CG_QUATERNION_IDENTITY, CG_FLOAT3_ONE * 0.01f);

	for (uint i = 0; i < count; ++i)
	{
		Graphics::DrawMesh(cornellBox.lock(), i, cornellBoxMaterial.lock(), matrix);
	}
	*/

	//Graphics::Blit(renderTarget->GetColorBuffer(0).lock(), Graphics::GetBackBuffer());
}

void DebugEngine::Step(GizmoRenderer* gizmos)
{
	auto aspect = Application::GetWindow().GetAspect();
	auto proj = CGMath::GetPerspective(50.0f, aspect, 0.1f, 50.0f);
	auto view = CGMath::GetMatrixInvTRS(CG_FLOAT3_ZERO, glm::quat(float3(0, Application::GetService<Time>()->GetTime(), 0)), CG_FLOAT3_ONE);
	auto vp = proj * view;

	gizmos->SetColor(CG_COLOR_GREEN);
	gizmos->DrawFrustrum(vp);
	
	FrustrumPlanes frustrum;
	CGMath::ExtractFrustrumPlanes(vp, &frustrum, true);

	auto cullables = m_entityDb->Query<PKECS::EntityViews::BaseRenderable>((int)PKECS::ENTITY_GROUPS::ACTIVE);

	for (auto i = 0; i < cullables.count; ++i)
	{
		const auto& bounds = cullables[i].bounds->aabb;
		gizmos->SetColor(CG_COLOR_BLACK);

		if (CGMath::IntersectPlanesAABB(frustrum.planes, 6, bounds))
		{
			gizmos->SetColor(CGMath::HueToRGB((float)i / (4 * 4 * 4)));
		}

		gizmos->DrawWireBounds(bounds.GetCenter(), bounds.GetExtents());
	}
}
