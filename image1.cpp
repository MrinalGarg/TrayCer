#include<iostream>
using namespace std;

const int ImgW =256;
const int ImgH =256;

int main()
{
	cout<<"P3\n"<<ImgW<<" "<<ImgH<<"\n255\n";

	for(int i=ImgH-1;i>=0;i--)
	{
		for(int j=0;j<ImgW;j++)
		{
			double r = double(j)/(ImgW-1);
			double g = double(i)/(ImgH-1);
			double b = 0.25;

			int ir = int(r*255.99);
			int ig = int(g*255.99);
			int ib = int(b*255.99);

			cout<<ir<<" "<<ig<<" "<<ib<<"\n";
		}
	}
	return 0;
}