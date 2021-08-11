int main(int argc, char** argv)
{
    int x = 0;				// this comment should go over the limit
  	int y = 1;

    for (int i = 0; 
	  	i < x + y; 
		i++) {
	y++;
	 x--;
    }
    return y;
}