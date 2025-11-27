// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectReview/MotionSequence/UI/MotionListWidget.h"
#include "MotionMainWidget.h"

void UMotionListWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//if (ParentMainWidget)
	//{
	//	FVector2D MainSize = ParentMainWidget->GetViewSize();
	//	FVector2D TargetPos = ParentMainWidget->GetViewPosition();
	//	TargetPos.X += MainSize.X;

	//	SetPositionInViewport(TargetPos, true);

	//	UE_LOG(LogTemp, Warning, TEXT("Hello: %s"), *TargetPos.ToString());
	//}
}


void UMotionListWidget::OnMainWidgetMoved(FVector2D Pos, FVector2D Size)
{
	FVector2D Target = Pos + FVector2D(Size.X, 0.f);
	SetPositionInViewport(Target, true);
}
