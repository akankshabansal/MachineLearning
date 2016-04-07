function h1Display(data, name)
%Function to display 6*6 images for report. 
baseImage=zeros(28*4, 28*4);
for i=1:4,
	for j=1:4,
		baseImage(1+(i-1)*28:i*28, 1+(j-1)*28:j*28) = data(:,:,((i-1)*4+j));
	end;
end;
imshow(baseImage);
imwrite(baseImage,name);
end

