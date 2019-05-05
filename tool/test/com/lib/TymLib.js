var crypto = require('crypto');
var log4js=require("log4js");
var fs = require("fs");
var bCheck=true;
var LogWho = "";
var LogLevel = "";
var CurDate=GetCurDay();
var defaultCompleteString = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";
var keyLen = 32;

function keyComplete(sKey){
    if(sKey.length % keyLen !=0){
        return sKey+defaultCompleteString.substr(0,keyLen - sKey.length);
    }
    return sKey;
}

function randDomString(iRandomLength){
    if(iRandomLength == 0 || iRandomLength == undefined){
        return "";
    }
    return Math.random().toString(36).substr(0,iRandomLength);
}

//计算MD5
function md5 (sSoure) {
    return crypto.createHash('md5').update(sSoure).digest('hex');
};
//获取当前绝对秒
function GetCurSecond(){
    var myDate = new Date();
    return parseInt(myDate.getTime()/1000);
}
//获取当前绝对毫秒
function GetCurMilliSecond(){
    var myDate = new Date();
    return parseInt(myDate.getTime());
}
//返回日期，20140308格式
function GetCurDay(){
    var myDate = new Date();
    var month = myDate.getMonth() + 1;
    month = month.toString();
    if(month.length == 1){
        month = "0" + month;
    }
    var date = myDate.getDate().toString();
    if( date.length == 1){
        date = "0" + date;
    }
    return myDate.getFullYear().toString()+ month + date;
}

//base64 endcode
function EncodeBase64(sSoure){
    var tmp = new Buffer(sSoure);
    return tmp.toString('base64');
}
//base64 decode
function DecodeBase64(sSoure){
    var tmp = new Buffer(sSoure, 'base64')
    return  tmp.toString();
}

//安全检验
function SafeCheck(psAppid,psToken,pSid,pAuthorization){
    if( !bCheck){
        return 0;
    }
    if( typeof(psAppid) != "string" ||  typeof(psToken) != "string" || typeof(pSid) != "string" || typeof(pAuthorization) != "string"){
        Log("info","SafeCheck param err:" + psAppid,psToken,pSid,pAuthorization);
        return -1;
    }

    var sAuthorization = DecodeBase64(pAuthorization);
    Log("debug","DecodeBase64 sAuthorization:" + sAuthorization);
    var iLen = psAppid.length;
    var sSecond = sAuthorization.substr(iLen);
    var sSid = md5(psAppid + psToken + sSecond);
    if( pSid != sSid){
        Log("info","SafeCheck sSid err:" + sSecond + " " + sSid  + " " + pSid);
        return -1;
    }
    var tSecondeTime = GetCurSecond();
    if( Math.abs(tSecondeTime - parseInt(sSecond)) > 60){//请求有效时间60秒
        Log("info","SafeCheck time out err");
        return -1;
    }
    return 0;
}


//安全检验v2
function SafeCheckV2(psAppid,psTimeStamp,psSign, psPostData){
    if( !bCheck){
        return 0;
    }
    if( typeof(psAppid) != "string" ||  typeof(psTimeStamp) != "string" || typeof(psSign) != "string"  ){
        Log("info","SafeCheck param err:" + psAppid,psTimeStamp,psSign);
        return -1;
    }

     var tSecondeTime = GetCurSecond();
    if( Math.abs(tSecondeTime - parseInt(psTimeStamp)) > 60){//请求有效时间60秒
        Log("info","SafeCheck time out err");
        return -1;
    }
    var appKey = GetAppKeyByAppid( psAppid);
    if( appKey.iRet == -1){
        Log("info","SafeCheck cannot get appKey");
        return -1;
    }

    var sSid = md5(appKey.sAppKey + psPostData + psTimeStamp + psAppid);

    if( psSign != sSid){
        Log("info","SafeCheck err sSid:" + sSid  );
        return -1;
    }

    return 0;
}
// 获取APPKey和AES密码（根据APPID）
function GetAppKeyByAppid(psAppid){
    var appKey = {};
    if( psAppid == "425388c9c928c3d36856e67b6d86cc11"){
        appKey.iRet = 0;
        appKey.sAppKey = "53b7a63d019de6ba32ed2e7fe38dfa72";
        appAesPwd = "tym1234567890123tym1234567890123";
    }else{
        appKey.iRet = -1;
    }
    return appKey;
}

//应答出错信息
function ResponsErrMsg(pResponse,psErrMsg){
    var Result = new Object();
    Result.iRet = -1;
    Result.sMsg = psErrMsg;
    Log("info","ResponsErrMsg:" + Result.sMsg );
    var sJson = JSON.stringify(Result);
    //数据AES加密
    var sAes = AesCipher(sJson);
    pResponse.write(sAes);
    pResponse.end();
}

function AesCipher(psPlainChunk,psKey, piLen){
    if( arguments.length == 1){
        psKey = "tym1234567890123tym1234567890123";
        piLen = 0;
    }else if(arguments.length == 2 ){
        piLen = 0;
    }
    var iv = '\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0';
    var algorithm = 'aes-256-cbc';
    var key = keyComplete(psKey);
    psPlainChunk = randDomString(piLen) + psPlainChunk;
    var clearEncoding = 'utf8';
    var cipherEncoding = 'base64';// 'binary', 'base64'或'hex'
    var cipher = crypto.createCipheriv(algorithm, key,iv);
    var cipherChunks = [];
    cipherChunks.push(cipher.update(psPlainChunk, clearEncoding, cipherEncoding));
    cipherChunks.push(cipher.final(cipherEncoding));
    return cipherChunks.join('');
}

function DeAesCipher(psCipherChunk,psKey, piLen){

    if( arguments.length == 1){
        psKey = "tym1234567890123tym1234567890123";
        piLen = 0;
    }else if(arguments.length == 2 ){
        piLen = 0;
    }
    var iv = '\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0';
    var algorithm = 'aes-256-cbc';
    var key = keyComplete(psKey);
    var clearEncoding = 'utf8';
    var cipherEncoding = 'base64';// 'binary', 'base64'或'hex'
    var decipher = crypto.createDecipheriv(algorithm, key,iv);
    var plainChunks = [];
    try{
        plainChunks.push(decipher.update(psCipherChunk, cipherEncoding, clearEncoding));
        plainChunks.push(decipher.final(clearEncoding));
    }catch(err){
        Log("info","DeAesCipher error:" + err);
        return "";
    }

    var value =  plainChunks.join('');
    return  value.substr(piLen,value.length - piLen);
}
/*
  @ log
  @pLevel --debug or info or warn or fatal
  */
function Log(pLevel,psMsg){
    try{
        if(!fs.existsSync("/log/" + LogWho)){
            return;
        }
        var sCurDate = GetCurDay();
        //保证一天一个日志
        if( CurDate != sCurDate){
            CurDate = sCurDate;
            log4js.configure({
                                 appenders:   [{
                                         "type": "dateFile",                 // 日志文件类型，可以使用日期作为文件名的占位符
                                         "filename": "/log/" + LogWho,     // 日志文件名，可以设置相对路径或绝对路径
                                         "pattern": "/logyyyyMMdd.log",  // 占位符，紧跟在filename后面
                                         "absolute": true,                   // filename是否绝对路径
                                         "alwaysIncludePattern": true,       // 文件名是否始终包含占位符
                                         "category": "log"               // 记录器名
                                     } ],
                                 levels:{log: LogLevel}//日志级别
                             });
        }

        var logger=log4js.getLogger('log');
        console.log(psMsg);
        if(pLevel == "info"){
            logger.info(psMsg);
        }else if(pLevel == "debug"){
            logger.debug(psMsg);
        }else if(pLevel == "warn"){
            logger.warn(psMsg);
        }else {
            logger.fatal(psMsg);
        }
    }
    catch(err){
        console.log("Write Log err:" + err);
    }
}
//设置日志目录和级别
function SetLog(psWho,psLevel){
    LogWho = psWho;
    LogLevel = psLevel;
    if(!fs.existsSync("/log/" + LogWho)){
        return;
    }
    log4js.configure({
                         appenders:   [{
                                 "type": "dateFile",                 // 日志文件类型，可以使用日期作为文件名的占位符
                                 "filename": "/log/" + LogWho,     // 日志文件名，可以设置相对路径或绝对路径
                                 "pattern": "/logyyyyMMdd.log",  // 占位符，紧跟在filename后面
                                 "absolute": true,                   // filename是否绝对路径
                                 "alwaysIncludePattern": true,       // 文件名是否始终包含占位符
                                 "category": "log"               // 记录器名
                             } ],
                         levels:{log: LogLevel}//日志级别
                     });
}


exports.md5=md5;
exports.SafeCheck=SafeCheck;
exports.GetCurSecond=GetCurSecond;
exports.EncodeBase64=EncodeBase64;
exports.DecodeBase64=DecodeBase64;
exports.ResponsErrMsg=ResponsErrMsg;
exports.AesCipher=AesCipher;
exports.DeAesCipher=DeAesCipher;
exports.GetCurMilliSecond=GetCurMilliSecond;
exports.Log=Log;
exports.SetLog=SetLog;
exports.GetCurDay=GetCurDay;
exports.SafeCheckV2=SafeCheckV2;
exports.GetAppKeyByAppid=GetAppKeyByAppid;
