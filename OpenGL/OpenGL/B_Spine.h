#pragma once
#include<vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class B_Spine {

public:
	B_Spine(int count,int k, std::vector<glm::vec2> control_Point, std::vector<double> u_control);

	std::vector<glm::vec2> height_array;
	std::vector<glm::vec2> control_Point;
	std::vector<float> y_array;
	 std::vector<double> u_control;
	 void generate_u();
	 int k;
	 int count;
	 double max_y = 0;
	 double min_y = 0;
	 double max_x = 0;
	 double min_x = 0;
	double deBoor_Cox_iter(double u, int k, int ui);
	std::vector<glm::vec2> produce_height_array();
};