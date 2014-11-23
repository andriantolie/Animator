#include "BSplineEvaluator.h"
#include <assert.h>

void BSplineEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	std::vector<Point> deBoorPts = ptvCtrlPts;

	// make sure the evaluated curve points are empty from the start
	ptvEvaluatedCurvePts.clear();
	int iCtrlPtCount = deBoorPts.size();

	if (!bWrap){
		// if the curve is not wrapped, make the beginning and the end of the curve horizontal
		ptvEvaluatedCurvePts.push_back(Point(0.0, deBoorPts[0].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, deBoorPts[iCtrlPtCount - 1].y));

		//hack to control the endpoints
		ptvEvaluatedCurvePts.push_back(deBoorPts[0]);
		ptvEvaluatedCurvePts.push_back(deBoorPts[iCtrlPtCount - 1]);
	}
	else{

		// create shadow control points for wrapping
		double shadowPointTail0X = fAniLength + deBoorPts[0].x;
		double shadowPointTail0Y = deBoorPts[0].y;
		Point shadowPointTail0(shadowPointTail0X, shadowPointTail0Y);

		double shadowPointTail1X = fAniLength + deBoorPts[1].x;
		double shadowPointTail1Y = deBoorPts[1].y;
		Point shadowPointTail1(shadowPointTail1X, shadowPointTail1Y);

		double shadowPointTail2X = fAniLength + deBoorPts[2].x;
		double shadowPointTail2Y = deBoorPts[2].y;
		Point shadowPointTail2(shadowPointTail2X, shadowPointTail2Y);

		deBoorPts.push_back(shadowPointTail0);
		deBoorPts.push_back(shadowPointTail1);
		deBoorPts.push_back(shadowPointTail2);
		iCtrlPtCount = deBoorPts.size();

	}
	// begin displaying bspline
	for (int i = 0; i + 3 < iCtrlPtCount; i++){
		// bezier curves need 4 control point
		std::vector<Point> ctrlPts =  convertDeBoor(deBoorPts[i], deBoorPts[i + 1], deBoorPts[i + 2], deBoorPts[i + 3]);
		displayBezier(ctrlPts[0], ctrlPts[1], ctrlPts[2], ctrlPts[3], ptvEvaluatedCurvePts, fAniLength);
	}


}

void BSplineEvaluator::displayBezier(Point v0, Point v1, Point v2, Point v3, std::vector<Point>& ptvEvaluatedCurvePts, float fAniLength) const{
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

std::vector<Point> BSplineEvaluator::convertDeBoor(Point b0, Point b1, Point b2, Point b3) const{
	std::vector<Point> ctrlPts;
	double v0X = (b0.x + 4 * b1.x + b2.x) / 6;
	double v0Y = (b0.y + 4 * b1.y + b2.y) / 6;
	double v1X = (4 * b1.x + 2*b2.x) / 6;
	double v1Y = (4 * b1.y + 2*b2.y) / 6;
	double v2X = (2 * b1.x + 4 * b2.x) / 6;
	double v2Y = (2 * b1.y + 4 * b2.y) / 6;
	double v3X = (b1.x + 4 * b2.x + b3.x) / 6;
	double v3Y = (b1.y + 4 * b2.y + b3.y) / 6;

	Point v0(v0X, v0Y);
	Point v1(v1X, v1Y);
	Point v2(v2X, v2Y);
	Point v3(v3X, v3Y);
	
	ctrlPts.push_back(v0);
	ctrlPts.push_back(v1);
	ctrlPts.push_back(v2);
	ctrlPts.push_back(v3);

	return ctrlPts;

}