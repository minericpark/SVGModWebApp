'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname+'/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

let parserLib = ffi.Library('./libsvgparse', {
  'fileToJSON': ['string', ['string', 'string']],
  'verifyFile': ['bool', ['string', 'string']],
  'titleDescToJSON': ['string', ['string', 'string']],
  'SVGrectToJSON': ['string', ['string', 'string']],
  'SVGcircToJSON': ['string', ['string', 'string']],
  'SVGpathToJSON': ['string', ['string', 'string']],
  'SVGgroupToJSON': ['string', ['string', 'string']],
  'getSVGComponentAttr': ['string', ['string', 'string', 'int']],
  'modifyAttr': ['string', ['string', 'string', 'int', 'string', 'string']],
  'modifyTitle': ['string', ['string', 'string', 'string']],
  'modifyDescr': ['string', ['string', 'string', 'string']],
  'createSVGFile': ['string', ['string', 'string']],
});

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
 
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: '+err);
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 

//Sample endpoint
app.get('/someendpoint', function(req , res){
  let retStr = req.query.name1 + " " + req.query.name2;
  res.send({
    foo: retStr
  });
});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);

//Respond to POST requests that upload files to uploads/ directory
app.post('/uploadCustom', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
  let result;

  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send('Failed to upload file');
    }

    result = parserLib.verifyFile("uploads/" + uploadFile.name, "parser/" + "svg.xsd");
    if (result != true) {
      fs.unlinkSync ('uploads/' + uploadFile.name);
      req.method = 'get';
    } 
    res.redirect('/');
  });
});

//Load table here
app.get('/loadTable', function(req, res) {
  var fileList = [];

  const directoryPath = 'uploads/';

  fs.readdir(directoryPath, function (err, files) {
    if (err) {
      return console.log ('Unable to scan directory');
    }

    files.forEach(function (file) {
      //Parse specified file into datastring required for table
      let fileName = file;
      var fileDetails = JSON.parse(parserLib.fileToJSON("uploads/" + fileName, "parser/" + "svg.xsd"));
      let fileSize = Math.round(fs.statSync("uploads/" + fileName).size / 1000);

      let fileJSON = {fileName, fileSize, fileDetails};
      //console.log(fileJSON);
      //Put into the array
      //console.log(file);
      fileList.push (fileJSON); 
    });
    res.send({
      //Return the array
      SVGLog: fileList
    });
  });
});

//Load dropdown here
app.get('/loadDropDown', function(req, res) {
  var fileList = [];

  const directoryPath = 'uploads/';

  fs.readdir(directoryPath, function (err, files) {
    if (err) {
      return console.log ('Unable to scan directory');
    }

    files.forEach(function (file) {
      //Parse specified file into datastring required for table
      let fileName = file;
      //console.log(fileJSON);
      //Put into the array
      //console.log(file);
      fileList.push (fileName); 
    });
    res.send({
      //Return the array
      SVGLog: fileList
    });
  });
});

//Load file view here
app.get('/loadViewSVG', function(req, res) {
  const directoryPath = 'uploads/';
  var tmpReq;
  let fileStruct;

  //Look for file name within directory, grab file, turn into object, return data
  console.log("entered");

  fs.readdir(directoryPath, function (err, files) {
    if (err) {
      return console.log ('Unable to scan directory');
    }

    //Parse the query filename appropriately
    tmpReq = req.query.filename;//
    console.log(tmpReq);
    files.forEach(function (file) {
      //Parse specified file into data object required for table
      if (file == tmpReq) {
        console.log('found');
        let fileName = file;
        var titleDescObj = JSON.parse(parserLib.titleDescToJSON("uploads/" + fileName, "parser/" + "svg.xsd"));
        var rectListObj = JSON.parse(parserLib.SVGrectToJSON("uploads/" + fileName, "parser/" + "svg.xsd"));
        var circListObj = JSON.parse(parserLib.SVGcircToJSON("uploads/" + fileName, "parser/" + "svg.xsd"));
        var pathListObj = JSON.parse(parserLib.SVGpathToJSON("uploads/" + fileName, "parser/" + "svg.xsd"));
        var groupListObj = JSON.parse(parserLib.SVGgroupToJSON("uploads/" + fileName, "parser/" + "svg.xsd"));

        fileStruct = {fileName, titleDescObj, rectListObj, circListObj, pathListObj, groupListObj};
      }
    });

    res.send({
      //Return the array
      SVG: fileStruct
    });
  });
});

//Get attribute
app.get('/loadAttribute', function(req, res) {
  const directoryPath = 'uploads/';
  var tmpReq;
  var tmpIndex;
  let attrList;

  //Look for file name within directory, grab file, turn into object, return data
  console.log("entered");

  fs.readdir(directoryPath, function (err, files) {
    if (err) {
      return console.log ('Unable to scan directory');
    }

    //Parse the query filename appropriately
    tmpReq = req.query.filename;//
    console.log(tmpReq);
    tmpIndex = req.query.index;
    console.log(tmpIndex);
    files.forEach(function (file) {
      //Parse specified file into data object required for table
      if (file == tmpReq) {
        console.log('found');
        let fileName = file;
        attrList = JSON.parse(parserLib.getSVGComponentAttr("uploads/" + fileName, "parser/" + "svg.xsd", tmpIndex));
      }
    });

    res.send({
      //Return the array
      attributes: attrList
    });
  });
});

//Update attribute
app.get('/upAttribute', function(req, res) {
  const directoryPath = 'uploads/';
  var tmpReq;
  var tmpIndex;
  var tmpName;
  var tmpVal;
  let bool;

  //Look for file name within directory, grab file, turn into object, return data
  console.log("entered");

  fs.readdir(directoryPath, function (err, files) {
    if (err) {
      return console.log ('Unable to scan directory');
    }

    //Parse the query filename appropriately
    tmpReq = req.query.filename;
    tmpIndex = req.query.index;
    tmpName = req.query.name;
    tmpVal = req.query.value;
    files.forEach(function (file) {
      //Parse specified file into data object required for table
      if (file == tmpReq) {
        console.log('found');
        //Call parser Lib to run file editing function
        bool = JSON.parse(parserLib.modifyAttr("uploads/" + tmpReq, "parser/" + "svg.xsd", tmpIndex, tmpName, tmpVal));
      }
    });

    res.send({
      //Return the array
      boolean: bool
    });
  });
});

//Update title
app.get('/upTitle', function(req, res) {
  const directoryPath = 'uploads/';
  var tmpReq;
  var tmpTitle;
  let bool;

  //Look for file name within directory, grab file, turn into object, return data
  console.log("entered");

  fs.readdir(directoryPath, function (err, files) {
    if (err) {
      return console.log ('Unable to scan directory');
    }

    //Parse the query filename appropriately
    tmpReq = req.query.filename;
    tmpTitle = req.query.title;
    console.log(tmpTitle);
    files.forEach(function (file) {
      //Parse specified file into data object required for table
      if (file == tmpReq) {
        console.log('found');
        //Call parser Lib to run file editing function
        bool = JSON.parse(parserLib.modifyTitle("uploads/" + tmpReq, "parser/" + "svg.xsd", tmpTitle));
      }
    });

    res.send({
      //Return the array
      boolean: bool
    });
  });
});

//Update description
app.get('/upDesc', function(req, res) {
  const directoryPath = 'uploads/';
  var tmpReq;
  var tmpDesc;
  let bool;

  //Look for file name within directory, grab file, turn into object, return data
  console.log("entered");

  fs.readdir(directoryPath, function (err, files) {
    if (err) {
      return console.log ('Unable to scan directory');
    }

    //Parse the query filename appropriately
    tmpReq = req.query.filename;
    tmpDesc = req.query.desc;
    console.log(tmpDesc);
    files.forEach(function (file) {
      //Parse specified file into data object required for table
      if (file == tmpReq) {
        console.log('found');
        //Call parser Lib to run file editing function
        bool = JSON.parse(parserLib.modifyDescr("uploads/" + tmpReq, "parser/" + "svg.xsd", tmpDesc));
      }
    });

    res.send({
      //Return the array
      boolean: bool
    });
  });
});

//Check if SVG filename is valid
app.get('/checkNewSVG', function(req, res) {
  const directoryPath = 'uploads/';
  var tmpReq;
  var bool;

  //Look for file name within directory, grab file, turn into object, return data
  console.log("entered");

  fs.readdir(directoryPath, function (err, files) {
    if (err) {
      return console.log ('Unable to scan directory');
    }

    //Parse the query filename appropriately
    tmpReq = req.query.filename;
    console.log(tmpReq);
    files.forEach(function (file) {
      //Parse specified file into data object required for table
      if (file == tmpReq) {
        console.log('found');
        //Call parser Lib to run file editing function
        bool = false;
      }
    });

    if (tmpReq == "") {
      console.log('file empty');
      bool = false;
    }

    //If filename does not have a .svg property, fail
    if (!tmpReq.includes('.svg')) {
      bool = false;
    }

    //If file not detected or no errors, return true
    if (bool != false) {
      bool = true;
    }

    res.send({
      //Return the array
      boolean: bool
    });
  });
});


//Create new SVG
app.get('/createNewSVG', function(req, res) {
  const directoryPath = 'uploads/';
  var tmpReq;
  var bool;

  //Look for file name within directory, grab file, turn into object, return data
  console.log("entered");

  fs.readdir(directoryPath, function (err, files) {
    if (err) {
      return console.log ('Unable to scan directory');
    }

    //Parse the query filename appropriately
    tmpReq = req.query.filename;
    console.log(tmpReq);

    bool = JSON.parse(parserLib.createSVGFile("uploads/" + tmpReq, "parser/" + "svg.xsd"));

    res.send({
      //Return the array
      boolean: bool
    });
  });
});
