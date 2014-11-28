#ifndef INCLUDED_CATMULLROM_CURVE_EVALUATOR_H
#define INCLUDED_CATMULLROM_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"
#include "modelerglobals.h"
#include "modelerapp.h"

class CatmullRomEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
	void displayCatmullRom(Point v0, Point v1, Point v2, Point v3, std::vector<Point>& ptvEvaluatedCurvePts, float fAniLength) const;
	std::vector<Point> convertToBezier(Point p0, Point p1, Point p2, Point p3) const;
};

#endif