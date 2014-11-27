#include "C2InterpolationEvaluator.h"
#include <assert.h>

void C2InterpolationEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	std::vector<Point> ctrlPts = ptvCtrlPts;

	// make sure the evaluated curve points are empty from the start
	ptvEvaluatedCurvePts.clear();
	int iCtrlPtCount = ctrlPts.size();

	// find velocity vectors
	std::vector<Point> velocityVectors = calculateVelocity(ctrlPts, bWrap, fAniLength);

	if (!bWrap){
		// if the curve is not wrapped, make the beginning and the end of the curve horizontal
		ptvEvaluatedCurvePts.push_back(Point(0.0, ctrlPts[0].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ctrlPts[iCtrlPtCount - 1].y));

		ptvEvaluatedCurvePts.push_back(ctrlPts[0]);
		ptvEvaluatedCurvePts.push_back(ctrlPts[iCtrlPtCount - 1]);
	}
	else{
		displayC2(Point(ctrlPts[iCtrlPtCount - 1].x - fAniLength, ctrlPts[iCtrlPtCount - 1].y), ctrlPts[0], velocityVectors[iCtrlPtCount - 1], velocityVectors[0], ptvEvaluatedCurvePts, fAniLength);
		displayC2(ctrlPts[iCtrlPtCount - 1], Point(ctrlPts[0].x + fAniLength, ctrlPts[0].y), velocityVectors[iCtrlPtCount - 1], velocityVectors[0], ptvEvaluatedCurvePts, fAniLength);

	}
	
	for (int i = 0; i + 1 < iCtrlPtCount; i++){
			displayC2(ctrlPts[i], ctrlPts[i + 1], velocityVectors[i], velocityVectors[i + 1], ptvEvaluatedCurvePts, fAniLength);
	}

}

void C2InterpolationEvaluator::displayC2(Point c0, Point c1, Point d0, Point d1, std::vector<Point>& ptvEvaluatedCurvePts, float fAniLength) const{
	Point v0(c0);
	Point v1(c0.x + d0.x / 3, c0.y + d0.y / 3);
	Point v2(c1.x - d1.x / 3, c1.y - d1.y / 3);
	Point v3(c1);
	for (double u = 0; u < 1.0; u += 0.01){
		Point v0Prime((1 - u)*v0.x + u*v1.x, (1 - u)*v0.y + u*v1.y);
		Point v1Prime((1 - u)*v1.x + u*v2.x, (1 - u)*v1.y + u*v2.y);
		Point v2Prime((1 - u)*v2.x + u*v3.x, (1 - u)*v2.y + u*v3.y);
		Point v0DoublePrime((1 - u)*v0Prime.x + u*v1Prime.x, (1 - u)*v0Prime.y + u*v1Prime.y);
		Point v1DoublePrime((1 - u)*v1Prime.x + u*v2Prime.x, (1 - u)*v1Prime.y + u*v2Prime.y);

		double resultX = (1 - u)*v0DoublePrime.x + u*v1DoublePrime.x;
		double resultY = (1 - u)*v0DoublePrime.y + u*v1DoublePrime.y;
		if (resultX <= fAniLength && resultX >= 0){
			Point result(resultX, resultY);
			ptvEvaluatedCurvePts.push_back(result);
		}
	}

}

std::vector<Point> C2InterpolationEvaluator::calculateVelocity(std::vector<Point> ctrlPts, bool isWrap, double fAniLength) const{

	const int ctrlPtCnt = ctrlPts.size();
	double** matrix = new double*[ctrlPtCnt];
	for (int i = 0; i < ctrlPtCnt; i++){
		matrix[i] = new double[ctrlPtCnt];
	}

	if (ctrlPtCnt >= 2){
		// initialize matrix start and end
		if (isWrap){
			matrix[0][0] = 4;
			matrix[0][1] = 1;
			matrix[0][ctrlPtCnt - 1] = 1;
			matrix[ctrlPtCnt - 1][ctrlPtCnt - 2] = 1;
			matrix[ctrlPtCnt - 1][ctrlPtCnt - 1] = 4;
			matrix[ctrlPtCnt - 1][0] = 1;
		}
		else{

			matrix[0][0] = 2;
			matrix[0][1] = 1;
			matrix[ctrlPtCnt - 1][ctrlPtCnt - 2] = 1;
			matrix[ctrlPtCnt - 1][ctrlPtCnt - 1] = 2;
		}


		// initialize middle of the matrix
		for (int i = 1; i < ctrlPtCnt - 1; i++){
			matrix[i][i - 1] = 1;
			matrix[i][i] = 4;
			matrix[i][i + 1] = 1;
		}

		//build right part of matrix
		std::vector<Point> rightPartOfMatrix;
		rightPartOfMatrix.clear();
		if (isWrap){
			rightPartOfMatrix.push_back(Point(3 * (ctrlPts[1].x - (ctrlPts[ctrlPtCnt-1].x - fAniLength)), 3 * (ctrlPts[1].y - ctrlPts[ctrlPtCnt-1].y)));
		}
		else{
			rightPartOfMatrix.push_back(Point(3 * (ctrlPts[1].x - ctrlPts[0].x), 3 * (ctrlPts[1].y - ctrlPts[0].y)));
		}
		
		for (int i = 1; i < ctrlPtCnt-1; i++){
			rightPartOfMatrix.push_back(Point(3 * (ctrlPts[i+1].x - ctrlPts[i-1].x), 3 * (ctrlPts[i+1].y - ctrlPts[i-1].y)));
		}

		if (isWrap){
			rightPartOfMatrix.push_back(Point(3 * (ctrlPts[0].x - (ctrlPts[ctrlPtCnt - 2].x-fAniLength)), 3 * (ctrlPts[0].y - ctrlPts[ctrlPtCnt - 2].y)));
		}
		else{
			rightPartOfMatrix.push_back(Point(3 * (ctrlPts[ctrlPtCnt - 1].x - ctrlPts[ctrlPtCnt - 2].x), 3 * (ctrlPts[ctrlPtCnt - 1].y - ctrlPts[ctrlPtCnt - 2].y)));
		}

		// build upper triangle matrix
		for (int i = 0; i < ctrlPtCnt; i++){
			double multiplyingFactor = 1.0 / matrix[i][i];
			for (int j = 0; j < ctrlPtCnt; j++){
				matrix[i][j] *= multiplyingFactor;
				for (int k = i + 1; k < ctrlPtCnt; k++){
					matrix[k][j] -= matrix[i][j] * matrix[k][i];
				}

			}

			rightPartOfMatrix[i].x *= multiplyingFactor;
			rightPartOfMatrix[i].y *= multiplyingFactor;

			for (int k = i + 1; k < ctrlPtCnt; k++){

				rightPartOfMatrix[k].x -= rightPartOfMatrix[i].x * matrix[k][i];
				rightPartOfMatrix[k].y -= rightPartOfMatrix[i].y * matrix[k][i];
			}

		}
		
		// build diagonal matrix
		for (int i = ctrlPtCnt - 1; i > 0; i--){
			double multiplyingFactor = matrix[i-1][i];
			for (int j = 0; j < ctrlPtCnt; j++){

				for (int k = i - 1; k >= 0; k--){
					matrix[k][j] -= matrix[i][j] * matrix[k][i];
				}
			}

			for (int k = i - 1; k >= 0; k--){
				rightPartOfMatrix[k].x -= rightPartOfMatrix[i].x * matrix[k][i];
				rightPartOfMatrix[k].y -= rightPartOfMatrix[i].y * matrix[k][i];
			}

		}

		// delete the matrix
		for (int i = 0; i < ctrlPtCnt; i++) {
			delete[] matrix[i];
		}
		delete[] matrix;

		return rightPartOfMatrix;
		
	}
}