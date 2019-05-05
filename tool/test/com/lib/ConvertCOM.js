// **********************************************************************
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
// **********************************************************************
//<script type="text/javascript" src="../../COM/ice/lib/Long.js"></script>
//<script type="text/javascript" src="../../COM/ice/lib/Ice.js"></script>
//类定义
var LongCv = require("./Long.js");
var Ice = require("./ice").Ice;
var ConvertCOM = (function() {
    //公开方法-把Long值转换成ICE使用的Long
    var LongToIceLong = function(pLong) {
        var longVal = LongCv.fromNumber(pLong);
        return new Ice.Long(longVal.high, longVal.low);
    }
    var lowAndHeightToICELong = function(low, high) {
        return new Ice.Long(high, low);
    }

    return {
        "lowAndHeightToICELong" : lowAndHeightToICELong,
        "LongToIceLong" : LongToIceLong
    };
}); 

exports.ConvertCOM=ConvertCOM;
