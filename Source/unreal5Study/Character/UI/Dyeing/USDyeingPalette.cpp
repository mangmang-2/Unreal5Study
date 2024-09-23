// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UI/Dyeing/USDyeingPalette.h"
#include "Components/Border.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Math/Vector.h"
#include "Engine/Texture2D.h"
#include "Rendering/Texture2DResource.h"
#include "Components/Image.h"
#include "USDyeingData.h"

// HSV to RGB 변환 함수 정의
FLinearColor HSVtoRGB(float H, float S, float V)
{
    FVector K(1.0f, 2.0f / 3.0f, 1.0f / 3.0f);

    FVector FracHSV(
        FMath::Frac(H + K.X),
        FMath::Frac(H + K.Y),
        FMath::Frac(H + K.Z)
    );

    FVector p = FracHSV * 6.0f - FVector(3.0f, 3.0f, 3.0f);
    p = p.GetAbs();

    FVector LerpResult(
        FMath::Lerp(K.X, FMath::Clamp(p.X - K.X, 0.0f, 1.0f), S),
        FMath::Lerp(K.Y, FMath::Clamp(p.Y - K.Y, 0.0f, 1.0f), S),
        FMath::Lerp(K.Z, FMath::Clamp(p.Z - K.Z, 0.0f, 1.0f), S)
    );

    FVector RGB = V * LerpResult;
    return FLinearColor(RGB.X, RGB.Y, RGB.Z, 1.0f);
}

void ChangeTexturePixel(UTexture2D* Texture, TArray<FColor> Pixels, int32 Width, int32 Height)
{
    // Mip 데이터 접근
    FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];

    void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
    FColor* FormData = reinterpret_cast<FColor*>(Data);  // 리소스 데이터를 FColor 배열로 캐스팅

    if (FormData)
    {
        for (int32 Y = 0; Y < Height; ++Y)
        {
            for (int32 X = 0; X < Width; ++X)
            {
                int32 Index = Y * Width + X;
                FormData[Index] = Pixels[Index];
            }
        }
    }

    // 데이터 잠금 해제
    Mip.BulkData.Unlock();

    // 텍스처 리소스 업데이트
    Texture->UpdateResource();
}

// 그라데이션 텍스처 생성 함수
UTexture2D* UUSDyeingPalette::CreateGradationTexture(UObject* Outer, int32 Width, int32 Height)
{
    // 새 텍스처 생성
    UTexture2D* Texture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
    if (!Texture)
    {
        return nullptr;
    }

    // 텍스처 설정
    Texture->MipGenSettings = TMGS_NoMipmaps;
    Texture->CompressionSettings = TC_VectorDisplacementmap;
    Texture->SRGB = true;
    Texture->AddToRoot(); // 가비지 컬렉션 방지
    Texture->UpdateResource();

    // 텍스처 데이터 버퍼 생성
    Pixels.SetNum(Width * Height);

    // 그라데이션 생성: 수평 방향으로 HSV Hue 값 변화
    for (int32 Y = 0; Y < Height; ++Y)
    {
        for (int32 X = 0; X < Width; ++X)
        {
            float H = static_cast<float>(X) / static_cast<float>(Width); // Hue 값은 0에서 1 사이
            float S = 1.0f; // 채도 고정
            float V = 1.0f; // 명도 고정

            // HSV -> RGB 변환
            FLinearColor LinearColor = HSVtoRGB(H, S, V);
            FColor Color = LinearColor.ToFColor(false);

            Pixels[Y * Width + X] = LinearColor.ToFColor(false);
        }
    }

    // 텍스처 업데이트
    ChangeTexturePixel(Texture, Pixels, Width, Height);

    return Texture;
}

UUSDyeingPalette::UUSDyeingPalette(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    WidgetID = EWidgetID::DyeingPalette;
}

void UUSDyeingPalette::NativeConstruct()
{
	Super::NativeConstruct();

    if (Palette && ColorMaterial)
    {
        // 머티리얼 인스턴스를 생성
        UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(ColorMaterial, this);
        //DynamicMaterial->
        UTexture2D* GradationTexture = CreateGradationTexture(this, TextureSize, TextureSize);
        DynamicMaterial->SetTextureParameterValue("Palette", GradationTexture);
        // Border에 머티리얼 적용
        Palette->SetBrushFromMaterial(DynamicMaterial);
    }
}

FReply UUSDyeingPalette::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    // 마우스 클릭 이벤트 처리
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        FVector2D ClickedPosition = InMouseEvent.GetScreenSpacePosition();
        UE_LOG(LogTemp, Log, TEXT("Mouse Clicked at: X=%f, Y=%f"), ClickedPosition.X, ClickedPosition.Y);
  
        // 마우스 이벤트에서 화면 좌표 가져오기
        FVector2D ScreenSpacePosition = InMouseEvent.GetScreenSpacePosition();

        // 화면 좌표를 위젯 로컬 좌표로 변환
        FVector2D LocalWidgetPosition = InGeometry.AbsoluteToLocal(ScreenSpacePosition);

        FColor NewColor = GetPixelColor(LocalWidgetPosition.X, LocalWidgetPosition.Y);

        //FLinearColor LinearColor = NewColor.ReinterpretAsLinear();
        //SelectColor->SetColorAndOpacity(LinearColor);

        ChangeModulPartsColor(NewColor);
    }

    return FReply::Handled();
}

// 텍스처에서 픽셀의 색상을 읽어오는 함수
FColor UUSDyeingPalette::GetPixelColor(int32 X, int32 Y)
{
    if (X < 0 || Y < 0 || X >= TextureSize || Y >= TextureSize)
        return FColor::Black; // 유효하지 않은 경우 기본 검정색 반환

    return Pixels[Y * TextureSize + X];
}

void UUSDyeingPalette::ChangeModulPartsColor(FColor Color)
{
    UDyeingMessage* WidgetMessage = NewObject<UDyeingMessage>();
    WidgetMessage->Color = Color.ReinterpretAsLinear();
    SendMessage(EWidgetID::DyeingPanel, 2, WidgetMessage);
}