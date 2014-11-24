#ifndef INCLUDED_BSPLINE_EVALUATOR_H
#define INCLUDED_BSPLINE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"

//using namespace std;

class BSplineEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
	void displayBezier(Point v0, Point v1, Point v2, Point v3, std::vector<Point>& ptvEvaluatedCurvePts, float fAniLength) const;
	std::vector<Point> convertDeBoor(Point b0, Point b1, Point b2, Point b3) const;
};

#endif