%$Id$
%-------------------------------------------------------------------------------
% function CallGMAT(cmd, varargin)
%-------------------------------------------------------------------------------
% GMAT: General Mission Analysis Tool
%
% Author: Linda Jun (NASA/GSFC)
% Created: 2003/03/03
%
% To form command string from command name and arguments and send command string
% to GMAT via ddepoke on Windows platform.
%    command string = cmd varargin;
%
% Since varargin is a {1xM} cell, it needs to convert it to a string.
% for example 'Create Spacecraft sat1' will have
%    cmd = 'Create' and varargin = 'Spacecraft' 'sat1'
% and command string will have
%    'Create Spacecraft sat1;'
%
% @param  cmd  input command name (such as Create, Propagate, GMAT)
% @param  varargin  command arguments (such as DefaultProp(DefaultSC))
%-------------------------------------------------------------------------------
% Copyright (c) 2002 - 2011 United States Government as represented by the
% Administrator of the National Aeronautics and Space Administration.
% All Other Rights Reserved.

function CallGMAT(cmd, varargin)

%%disp('==============================');
%%disp([cmd varargin{1}]);

% convert {1xM} cell array of arguments to array of strings of arguments
strarr = char(varargin{1});
%%disp(strarr);

%-------------------------------------------------
% concatenate command and command arguments
%-------------------------------------------------
m = size(strarr, 1);

% add space after command if number of arguments is greater than zero
str = cmd;
if m > 0
   str = [cmd ' '];
end
%%disp(str);

for i = 1:m
   str1 = strarr(i,:);
   str1 = deblank(str1);
   str = [str str1];
   if i < m
      str = [str ' '];
   end
end

str = [str ';'];
%%disp(str);

% Check if it is unix (MacOS X or Linux)
if isunix
   SendGMAT('Poke',str); 
   return;
end

%-------------------------------------------------
% call dde function to send string
%-------------------------------------------------
global gmatChannel;

Poke(gmatChannel, 'script', str);
