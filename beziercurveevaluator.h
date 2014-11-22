#ifndef INCLUDED_BEZIER_CURVE_EVALUATOR_H
#define INCLUDED_BEZIER_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"

//using namespace std;

class BezierCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
	void displayBezier(Point v0, Point v1, Point v2, Point v3, std::vector<Point>& ptvEvaluatedCurvePts, float fAniLength) const;
};

#endif