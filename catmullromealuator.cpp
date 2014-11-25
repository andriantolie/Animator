#include "catmullromevaluator.h"
#include <assert.h>
#include <iostream>

void CatmullRomEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
	std::vector<Point>& ptvEvaluatedCurvePts, const float& fAniLength, const bool& bWrap) const {

	std::vector<Point> tmpCtrlPts = ptvCtrlPts;

	ptvEvaluatedCurvePts.clear();
	int iCtrlPtCount = tmpCtrlPts.size();

	if (!bWrap) {
		ptvEvaluatedCurvePts.push_back(Point(0.0, tmpCtrlPts[0].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, tmpCtrlPts[iCtrlPtCount - 1].y));

		ptvEvaluatedCurvePts.push_back(tmpCtrlPts[0]);
		ptvEvaluatedCurvePts.push_back(tmpCtrlPts[iCtrlPtCount - 1]);
	}
	else {
		// create two shadow control points for warpping
		double shadowPointTail0X = fAniLength + tmpCtrlPts[0].x;
		double shadowPointTail0Y = tmpCtrlPts[0].y;
		Point shadowPointTail0(shadowPointTail0X, shadowPointTail0Y);

		double shadowPointTail1X = fAniLength + tmpCtrlPts[1].x;
		double shadowPointTail1Y = tmpCtrlPts[1].y;
		Point shadowPointTail1(shadowPointTail1X, shadowPointTail1Y);

		double shadowPointTail2X = fAniLength + tmpCtrlPts[2].x;
		double shadowPointTail2Y = tmpCtrlPts[2].y;
		Point shadowPointTail2(shadowPointTail2X, shadowPointTail2Y);

		tmpCtrlPts.push_back(shadowPointTail0);
		tmpCtrlPts.push_back(shadowPointTail1);
		tmpCtrlPts.push_back(shadowPointTail2);
		iCtrlPtCount = tmpCtrlPts.size();
	}
	
	for (int i = 0; i + 3 < iCtrlPtCount; i++){
		// bezier curves need 4 control point
		std::vector<Point> ctrlPts = convertToBezier(tmpCtrlPts[i], tmpCtrlPts[i + 1], tmpCtrlPts[i + 2], tmpCtrlPts[i + 3]);
		displayCatmullRom(ctrlPts[0], ctrlPts[1], ctrlPts[2], ctrlPts[3], ptvEvaluatedCurvePts, fAniLength);
	}

}

void CatmullRomEvaluator::displayCatmullRom(Point v0, Point v1, Point v2, Point v3, std::vector<Point>& ptvEvaluatedCurvePts, float fAniLength) const {
	/*int i;
	double sigma = 1.0 / 64;
	double t = 0.0;

	double cx[4] = { 1 / 2 * (-1 * v0.x + 3 * v1.x - 3 * v2.x + v3.x), 1 / 2 * (2 * v0.x - 5 * v1.x + 4 * v2.x - v3.x), 1 / 2 * (-v0.x + v2.x), 1 / 2 * (2 * v1.x) };
	double cy[4] = { 1 / 2 * (-1 * v0.y + 3 * v1.y - 3 * v2.y + v3.y), 1 / 2 * (2 * v0.y - 5 * v1.y + 4 * v2.y - v3.y), 1 / 2 * (-v0.y + v2.y), 1 / 2 * (2 * v1.y) };

	for (i = 0; i < 64; ++i) {
		double t2, t3, x, y;
		t += sigma;
		t2 = t * t;
		t3 = t2 * t;
		x = cx[0] * t3 + cx[1] * t2 + cx[2] * t + cx[3];
		y = cy[0] * t3 + cy[1] * t2 + cy[2] * t + cy[3];
		Point result(x, y);
		ptvEvaluatedCurvePts.push_back(result);*/
	//std::cout << "v0:" << v0 << " v1:" << v1 << " v2:" << v2 << " v3:" << v3 << std::endl;
	for (double u = 0.0f; u < 1.0; u += 0.01){
		Point v0Prime((1 - u)*v0.x + u*v1.x, (1 - u)*v0.y + u*v1.y);
		Point v1Prime((1 - u)*v1.x + u*v2.x, (1 - u)*v1.y + u*v2.y);
		Point v2Prime((1 - u)*v2.x + u*v3.x, (1 - u)*v2.y + u*v3.y);
		Point v0DoublePrime((1 - u)*v0Prime.x + u*v1Prime.x, (1 - u)*v0Prime.y + u*v1Prime.y);
		Point v1DoublePrime((1 - u)*v1Prime.x + u*v2Prime.x, (1 - u)*v1Prime.y + u*v2Prime.y);
		//std::cout << "v0prime" << v0Prime << " vodouleprime:" << v0DoublePrime << std::endl;
		double resultX = (1 - u)*v0DoublePrime.x + u*v1DoublePrime.x;
		double resultY = (1 - u)*v0DoublePrime.y + u*v1DoublePrime.y;
		if (resultX >= fAniLength) resultX = resultX - fAniLength;
		Point result(resultX, resultY);
		
		ptvEvaluatedCurvePts.push_back(result);
	}
}

std::vector<Point> CatmullRomEvaluator::convertToBezier(Point p0, Point p1, Point p2, Point p3) const {
	std::vector<Point> ctrlPts;
	/*Point v0(p1.x, p1.y);
	Point v1(p1.x + 1 / 6 * (p2.x - p0.x), p1.y + 1 / 6 * (p2.y - p0.y));
	Point v2(p2.x - 1 / 6 * (p3.x - p1.x), p2.y - 1 / 6 * (p3.y - p1.y));
	Point v3(p2.x, p2.y);*/
	double v0X = p1.x;
	double v0Y = p1.y;
	double v1X = p1.x +  (p2.x - p0.x) / 6;
	//std::cout << "p2-p0: " << (p2.x - p0.x) / 6 << std::endl;
	double v1Y = p1.y +  (p2.y - p0.y) / 6;
	double v2X = p2.x -  (p3.x - p1.x) / 6;
	double v2Y = p2.y -  (p3.y - p1.y) / 6;
	double v3X = p2.x;
	double v3Y = p2.y;
	
	//std::cout << v0X << " " << v0Y << " " << v1X << " " << v1Y << " " << v2X << " " << v2Y << " " << v3X << " " << v3Y << std::endl;

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