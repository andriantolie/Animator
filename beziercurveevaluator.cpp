#include "BezierCurveEvaluator.h"
#include <assert.h>

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{   
	std::vector<Point> ctrlPts = ptvCtrlPts;

	// make sure the evaluated curve points are empty from the start
	ptvEvaluatedCurvePts.clear();
	int iCtrlPtCount = ctrlPts.size();

	if (!bWrap){
		// if the curve is not wrapped, make the beginning and the end of the curve horizontal
		ptvEvaluatedCurvePts.push_back(Point(0.0, ctrlPts[0].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ctrlPts[iCtrlPtCount - 1].y));
	}
	else{
		// in case wrapping forms a new complete bezier curve
		if (iCtrlPtCount % 3 == 0){
			// create shadow control points for wrapping
			double shadowPointTailX = fAniLength + ctrlPts[0].x;
			double shadowPointTailY = ctrlPts[0].y;
			Point shadowPointTail(shadowPointTailX, shadowPointTailY);
			ctrlPts.push_back(shadowPointTail);
			iCtrlPtCount = ctrlPts.size();
		}
		// otherwise treat it the interpolation the same as linear curve
		else{
			float newY;
			if ((ctrlPts[0].x + fAniLength) - ctrlPts[iCtrlPtCount - 1].x > 0.0f) {
				newY = (ctrlPts[0].y * (fAniLength - ctrlPts[iCtrlPtCount - 1].x) +
					ctrlPts[iCtrlPtCount - 1].y * ctrlPts[0].x) /
					(ctrlPts[0].x + fAniLength - ctrlPts[iCtrlPtCount - 1].x);
			}
			else{
				newY = ctrlPts[0].y;
			}
			ptvEvaluatedCurvePts.push_back(Point(0.0, newY));
			ptvEvaluatedCurvePts.push_back(Point(fAniLength, newY));

		}
		
	}
	// begin displaying bezier curves
	int i = 0;
	for (; i + 3 < iCtrlPtCount; i += 3){
		// bezier curves need 4 control point
			displayBezier(ctrlPts[i], ctrlPts[i + 1], ctrlPts[i + 2], ctrlPts[i + 3], ptvEvaluatedCurvePts, fAniLength);
	}

	// add the remaining control points to the evaluated result
	for (; i < iCtrlPtCount; i++){
		ptvEvaluatedCurvePts.push_back(ctrlPts[i]);
	}

}

void BezierCurveEvaluator::displayBezier(Point v0, Point v1, Point v2, Point v3, std::vector<Point>& ptvEvaluatedCurvePts, float fAniLength) const{
	for (double u = 0; u < 1.0; u += 0.01){
		Point v0Prime((1 - u)*v0.x + u*v1.x, (1 - u)*v0.y + u*v1.y);
		Point v1Prime((1 - u)*v1.x + u*v2.x, (1 - u)*v1.y + u*v2.y);
		Point v2Prime((1 - u)*v2.x + u*v3.x, (1 - u)*v2.y + u*v3.y);
		Point v0DoublePrime((1 - u)*v0Prime.x + u*v1Prime.x, (1 - u)*v0Prime.y + u*v1Prime.y);
		Point v1DoublePrime((1 - u)*v1Prime.x + u*v2Prime.x, (1 - u)*v1Prime.y + u*v2Prime.y);

		double resultX = (1 - u)*v0DoublePrime.x + u*v1DoublePrime.x;
		double resultY = (1 - u)*v0DoublePrime.y + u*v1DoublePrime.y;
		if (resultX >= fAniLength) resultX = resultX - fAniLength;
			Point result(resultX, resultY);
			ptvEvaluatedCurvePts.push_back(result);
		
	}

}
