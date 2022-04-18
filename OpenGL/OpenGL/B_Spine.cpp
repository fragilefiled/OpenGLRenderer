#include "B_Spine.h"

B_Spine::B_Spine(int count, int k, std::vector<glm::vec2> control_Point, std::vector<double> u_control)
{
	this->k = k;
	this->count = count;
	height_array=std::vector<glm::vec2>(count);
	y_array = std::vector<float>(count*4,0); 
	this->control_Point =(control_Point);
	this->u_control =(u_control);
}

void B_Spine::generate_u()
{
	this->u_control = std::vector<double>(control_Point.size() + k + 1, 0);
		for (int i = 0; i < control_Point.size()+k+1; i++){
			double temp = (float)i / float(control_Point.size() + k);
			u_control[i] = temp;

		}
}

double B_Spine::deBoor_Cox_iter(double u, int k, int i)
{
	
	double coef_1 = 0;
	double coef_2 = 0;

	if (k == 1)
		if (u >= (u_control)[i] && u <= (u_control)[i + 1])
			return 1;
		else
			return 0.0;

	else {
		double ui = (u_control)[i];
		double uik = (u_control)[i + k];
		double uik_1 = (u_control)[i + k - 1];
		double ui1 = (u_control)[i + 1];

		coef_1 = (u - ui) / (uik_1 - ui);
		coef_2 = (uik - u) / (uik - ui1);
		if (uik_1 - ui == 0)
			coef_1 = 0;
		if (uik - ui1 == 0)
			coef_2 = 0;
		return coef_1 * deBoor_Cox_iter(u, k-1, i) + coef_2 * deBoor_Cox_iter(u, k-1, i + 1);
	}
	
}


std::vector<glm::vec2> B_Spine::produce_height_array()
{	
	max_y = -1e12;
	min_y = 1e12;
	max_x = -1e12;
	min_x = 1e12;
	
	double u = double(k - 1 + 0.002);
	double initu = u;
	int n = control_Point.size();
	int count_in = 0;
	while (u <= n&&count_in<height_array.size()) {
		double y = 0;
		double x = 0;
		for (int i = 0; i < n; i++) {
			double b_ik = deBoor_Cox_iter(u / (double)(n + k), k, i);
			x += b_ik * control_Point[i].x;
			y += b_ik * control_Point[i].y;			
		}
		height_array[count_in].x = x;
		height_array[count_in].y = y;
		y_array[count_in * 4] = x;
		y_array[count_in * 4 +1] = y;
		y_array[count_in * 4 +2] = 0;
		y_array[count_in * 4 +3] = 0;
		max_y = std::max(max_y, y);
		min_y = std::min(min_y, y);
		max_x = std::max(max_x, x);
		min_x = std::min(min_x, x);
		count_in+=1;
		//u += 0.1;
		u += (n -initu) / ((double)height_array.size());

	}
	min_y = std::max(y_array[1], y_array[y_array.size() - 3]);
	//min_y = y_array[1];

	for (int i = 0; i < y_array.size(); i+=4) {
		y_array[i + 1] = std::max(min_y, (double)y_array[i + 1]);
		//y_array[i+1] =(y_array[i+1]-min_y)/ (max_y - min_y);
		//y_array[i]= (y_array[i] - min_x) / (max_x - min_x)-(float)i/4.0/(height_array.size()-1);
		y_array[i + 1] = (y_array[i + 1] - min_y) / (max_y - min_y) / (max_x - min_x);
		y_array[i] = (y_array[i] - min_x) / (max_x - min_x) / (max_y - min_y) - (float)i / 4.0 / (height_array.size() - 1) / (max_y - min_y);
		if (y_array[i + 1] == 0.0)
			y_array[i] = 0.0;
		
	}
	return height_array;
	// TODO: 在此处插入 return 语句
}
