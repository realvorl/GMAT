function Data = hs077()

%   Copyright 2002-2005, United States Government as represented by the 
%   Administrator of The National Aeronautics and Space Administration.
%   All Rights Reserved.
%   This software is licensed under the NASA Open Source Agreement.
%
%   Modification History
%   -----------------------------------------------------------------------
%   Sept-08-08  S. Hughes:  Created the initial version.

Data.x0  = [2 2 2 2 2]'/4;
Data.lb  = []'; 
Data.ub  = []';
Data.A   = [];
Data.b   = [];
Data.Aeq = [];
Data.beq = [];
Data.xstar = nan* [1 1 1 1 1]';
Data.fstar = 0.24150513;