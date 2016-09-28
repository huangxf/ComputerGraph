namespace math_tool {

	float clamp(float val)
	{
		if(std::abs(val) <= 0.0001) return 0;
		else return val;
	}

}