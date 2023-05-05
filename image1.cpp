#include<iostream>
#include"vec3.h"
#include"color.h"


const int ImgW =256;
const int ImgH =256;

int main()
{
	std::cout<<"P3\n"<<ImgW<<" "<<ImgH<<"\n255\n";

	for(int i=ImgH-1;i>=0;i--)
	{
		std::cerr << "\rScanlines remaining: " << i << ' ' << std::flush;
		for(int j=0;j<ImgW;j++)
		{
			color pc (double(j)/(ImgW-1),double(i)/(ImgH-1),0.25);

			write_color(std::cout,pc);

		}
	}

	std::cerr<<"\nDone\n";
	return 0;
}