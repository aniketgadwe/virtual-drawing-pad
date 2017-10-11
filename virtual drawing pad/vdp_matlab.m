
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%							%
%     Aim : Virtual Drawing Pad                         %
%     Author : Aniket : IvLabs				%	
%							%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% image acquisition set up
vid = videoinput('winvideo', 1, 'YUY2_320x240'); 
src = getselectedsource(vid); 
vid.FramesPerTrigger = Inf; 
vid.ReturnedColorspace = 'rgb';
triggerconfig(vid, 'immediate');
vid.FrameGrabInterval = 5;
result=zeros(240,320);
index=1;
counter=1;
bc=[252 209];
% start video
start(vid);
for(i=1:25)
    img=getsnapshot(vid);
    imshow(img);
end
stop(vid);
vid.FrameGrabInterval = 2;
pause(1);

start(vid);
while(vid.FramesAcquired<=400) 
    
    img1 = getsnapshot(vid);
    
    diff_img1 = imsubtract(img1(:,:,1), rgb2gray(img1));
    x = medfilt2(diff_img1, [3 3]);
    diff_img2 = im2bw(x,0.24);
    
    % Remove all those pixels less than 250px
    diff_img2 = bwareaopen(diff_img2,250);
    % Label all the connected components in the image.
    bw = bwlabel(diff_img2);
    stats = regionprops(bw, 'BoundingBox', 'Centroid');
    pause(0.2);
    %       figure,imshow(data)
    %       hold on
   
    %  get status value with time
    for object = 1:length(stats)
        bb = stats(object).BoundingBox;
        bc= stats(object).Centroid;
    end
    % to get continous lines 
    
   if (index == 1)
      p=bc(1,1);
      q=bc(1,2);
     

   index=2;
   end

   if(index ~= 1)
      r=bc(1,1);
      s=bc(1,2);
     
   end

   if(index ~=1)
      x=linspace(p,r,100);
      y=linspace(q,s,100);
        for k = 1:100
             a=x(1,k);
             b=y(1,k);
%                if (round(b)>0 && round(a)>0)
             result(round(b),round(a))=1;
%               end
        end
      p=r;
      q=s;
   end
   result1=flipdim(result,2);
   if(counter==2)
 imshow((result1))
   end
 counter=2;
 
%   pause(0.1);
%    hold off
end
stop(vid);
%flushdata(vid);
