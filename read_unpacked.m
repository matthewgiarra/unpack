
fclose all;
binDir = '~/Desktop/mng-1-161-A';
binName = 'mng-1-161-A';
binExt = '.bin';

imageHeight = 1024;
imageWidth  = 1024;
nImages = 1;

% Path to binary file
binPath = fullfile(binDir, [binName binExt]);

% Open binary file for reading
fid = fopen(binPath, 'r');

% Read the binary file
data = fread(fid, imageHeight * imageWidth * nImages, 'uint16');

% Reshape the data into a matrix
data_mat = reshape(data, imageHeight, imageWidth, nImages);
% 
% % Display the first image
imagesc(uint16(data_mat(:, :, 1))); axis image; colormap gray;


fclose(fid);