#ifndef INCLUDED_C2INTERPOLATION_EVALUATOR_H
#define INCLUDED_C2INTERPOLATION_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"

//using namespace std;

class C2InterpolationEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
	void displayC2(Point c0, Point c1, Point d0, Point d1, std::vector<Point>& ptvEvaluatedCurvePts, float fAniLength) const;
	std::vector<Point> calculateVelocity(std::vector<Point> ctrlPts, bool isWrap, double fAniLength) const;
};

#endif