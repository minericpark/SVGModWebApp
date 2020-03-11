// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
    // On page-load AJAX Example
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/someendpoint',   //The server endpoint we are connecting to
        data: {
            name1: "Value 1",
            name2: "Value 2"
        },
        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string, 
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */
            $('#blah').html("On page load, received string '"+data.foo+"' from server");
            //We write the object to the console to show that the request was successful
            console.log(data); 

        },
        fail: function(error) {
            // Non-200 return, do something with error
            $('#blah').html("On page load, received error from server");
            console.log(error); 
        }
    });

    //Ajax call that loads the initial SVG viewing table (calls loadTable)
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/loadTable',
        success: function(filelog) {
            console.log ("file log successfully loaded"); //Error message for console
            var filetable = '<table class="center">';
            var fileHeader = ["Image (click to download)", "File name (click to download)", "File size", "Number of rectangles", "Number of circles", "Number of paths", "Number of groups"];
            //If returned array length is 0, display alt message
            if (filelog.SVGLog.length == 0) {
                console.log("empty");
                filetable += '<tr>';
                filetable += '<td class="noFiles">' + 'No files';
                filetable += '</td></tr>';
                
            } else { //Otherwise, run for loop that creates the table,
                console.log("there exists files");
                //Set up header of table
                filetable += '<tr>';
                for (var i = 0; i < fileHeader.length; i++) {
                    filetable +='<th>' + fileHeader[i] + '</th>';
                }
                filetable += '</tr>';

                //Create cells for each files that exists within uploads
                console.log(filelog.SVGLog.length);
                for (var i = 0; i < filelog.SVGLog.length; i++) {
                    filetable += '<tr>';
                    //Use get??
                    filetable += '<td><a href="' + filelog.SVGLog[i].fileName + '">' + '<img src="' + filelog.SVGLog[i].fileName + '" class="logimage"></td>';
                    filetable += '<td><a href="' + filelog.SVGLog[i].fileName + '">' + filelog.SVGLog[i].fileName + '</td>';
                    filetable += '<td>' + filelog.SVGLog[i].fileSize + 'KB</td>';
                    filetable += '<td>' + filelog.SVGLog[i].fileDetails.numRect + '</td>';
                    filetable += '<td>' + filelog.SVGLog[i].fileDetails.numCirc + '</td>';
                    filetable += '<td>' + filelog.SVGLog[i].fileDetails.numPaths + '</td>';
                    filetable += '<td>' + filelog.SVGLog[i].fileDetails.numGroups + '</td>';
                    console.log(i);
                    filetable += '</tr>';
                }
            }
            filetable += "</table>";
            $('#filelog').html(filetable);
        },
        fail: function(error) {
            console.log("file log failed to load");
            console.log(error);
        }
    });

    //Ajax call that uploads files to server (calls uploadCustom)
    var form = document.getElementById('uploadFile');
        var formData = new FormData();
        formData.append ('file', $('input[type=file]')[0].files[0]);
        
        $('#submitfile').submit(function(e) {
            $.ajax({
                type: "post",
                url: '/uploadCustom',
                data: formData,
                contentType: false,
                processData: false,
                cache: false,
    
                success: function(data) {
                    console.log(data);
                    console.log("file successfully uploaded"); //Error message for console
                    //Print success message
                    alert("Successfully submitted SVG");
                },
                error: function(error) {
                    //Print error message
                    console.log("file failed to upload"); //Error message for console
                    alert("Invalid SVG or file");
                }
            });
        });

    //Ajax call that loads the dropdown menu
    $.ajax({
        type: "get",
        datatype: 'json',
        url: '/loadDropDown',
        success: function(filelog) {
            console.log("dropdown successfully loaded"); //Error message for console debugging
            var filedropdown = '<select id="images" class="svgdropdown">';
            //If returned array length is 0, disable dropdown list
            if (filelog.SVGLog.length == 0) {
                console.log("empty");
                filedropdown = '<select id="images" disabled>';
                filedropdown += '<option value="No files in server">No files in server</label>';
            } else { //Otherwise, run for loop that creates the dropdown,
                console.log("there exists files");
                //Set up all SVG options
                console.log(filelog.SVGLog.length);
                for (i = 0; i < filelog.SVGLog.length; i++) {
                    if (i === 0) { //Ensure first element of dropdown is selected
                        filedropdown += '<option selected value="' + filelog.SVGLog[i] + '">' + filelog.SVGLog[i] + '</label>';
                    } else {
                        filedropdown += '<option value="' + filelog.SVGLog[i] + '">' + filelog.SVGLog[i] + '</label>';
                    }
                    //Use get??
                    console.log(i);
                }
            }
            filedropdown += '</select>';
            $('#svgdropdown').html(filedropdown);
            //Trigger the svg view to load
            $('#svgdropdown').trigger("load", []);
        },
        error: function(error) {
            console.log("dropdown failed to load"); //Error message for console debugging
            console.log(error);
        }
    });

    //Loads svg table when dropdown loads
    $('#svgdropdown').on("load", function() {
        var detectFiles = null;
        var selected = null;
        var svgtable = "<table>";
            var svgHeader = ["Title", "Description", "Component", "Summary", "Other Attributes"];

        $.ajax({
            type: "get",
            datatype: 'json',
            url: '/loadDropDown',
            success: function(filelog) {
                console.log("server scanned");
                detectFiles = filelog.SVGLog.length;
                if (detectFiles == 0) { //File is empty, do not ajax call, just set svgtable to empty
                    //console.log("empty svg view");
                    //Create empty base table
                    svgtable += '<tr><th colspan="10" class="svgpanelimage"></th></tr>';
            
                    //Create first two headers
                    svgtable += '<tr>';
                    for (var i = 0; i < 2; i++) {
                        svgtable +='<th colspan="3">' + svgHeader[i] + '</th>';
                    }
                    svgtable += '</tr>';
    
                    //Add empty cells for title and description
                    svgtable += '<tr>';
                    svgtable += '<td colspan="3"></td>';
                    svgtable += '<td colspan="3"></td>';
                    svgtable += '<tr>';
    
                    //Add last 3 headers
                    svgtable += '<tr>';
                    for (var i = 2; i < svgHeader.length; i++) {
                        if (i == 3) {
                            svgtable += '<th colspan="4">' + svgHeader[i] + '</th>';
                        } else {
                            svgtable += '<th>' + svgHeader[i] + '</th>';
                        }
                    }
                    svgtable += '</tr>';

                    //Create an empty cell
                    svgtable += '<tr>';
                    svgtable += '<td></td>';
                    svgtable += '<td colspan="4"></td>';
                    svgtable += '<td></td>';
                    svgtable += '</tr>';

                    svgtable += "</table>";
                    //Update the SVG table 
                    $('#svgview').html(svgtable);
                } else { //File list is not empty
                    //Set selected value and use ajax call to detect file appropriately
                    selected = $('#images').children("option:selected").val();
                    $.ajax({
                        type: "get",
                        datatype: 'json',
                        data: {
                            filename: selected,
                        },
                        url: '/loadViewSVG',
                        success: function(fileStruct) {
                            //console.log("SVG view successfully loaded"); //Error message for console
                            //console.log(selected);
                            //console.log(fileStruct.SVG.fileName);
                            //console.log(fileStruct.SVG.titleDescObj);
                            //console.log(fileStruct.SVG.titleDescObj.title);
                            //console.log(fileStruct.SVG.titleDescObj.desc);
            
                            //Create picture
                            svgtable += '<tr><th colspan="10"><img src="' + 'uploads/' + fileStruct.SVG.fileName + '" class="svgpanelimage"></th></tr>';
            
                            //Create first two headers + buttons
                            svgtable += '<tr>';
                            for (var i = 0; i < 2; i++) {
                                svgtable +='<th colspan="3">' + svgHeader[i] +'</th>';
                            }
                            svgtable += '</tr>';
            
                            //Add cells for title and description
                            svgtable += '<tr>';
                            svgtable += '<td colspan="3">' + fileStruct.SVG.titleDescObj.title + '</td>';
                            svgtable += '<td colspan="3">' + fileStruct.SVG.titleDescObj.desc + '</td>';
                            svgtable += '<tr>';
            
                            //Add last 3 headers
                            svgtable += '<tr>';
                            for (var i = 2; i < svgHeader.length; i++) {
                                if (i == 3) {
                                    svgtable += '<th colspan="4">' + svgHeader[i] + '</th>';
                                } else {
                                    svgtable += '<th>' + svgHeader[i] + '</th>';
                                }
                            }
                            svgtable += '</tr>';
            
                            //Add appropriate cells for the file given
                            //start with rectangles, circles, paths, then group
                            for (var i = 0; i < fileStruct.SVG.rectListObj.length; i++) {
                                svgtable += '<tr>';
                                svgtable += '<td>Rectangle ' + (i+1) + '</td>';
                                if (fileStruct.SVG.rectListObj[i].units != "") {
                                    svgtable += '<td colspan="4">Upper left corner: x = ' + fileStruct.SVG.rectListObj[i].x + fileStruct.SVG.rectListObj[i].units;
                                    svgtable += ' y = ' + fileStruct.SVG.rectListObj[i].y + fileStruct.SVG.rectListObj[i].units;
                                    svgtable += '<br>Width = ' + fileStruct.SVG.rectListObj[i].w + fileStruct.SVG.rectListObj[i].units;
                                    svgtable += ' Height = ' + fileStruct.SVG.rectListObj[i].h + fileStruct.SVG.rectListObj[i].units;
                                    svgtable += '</td>';
                                } else {
                                    svgtable += '<td colspan="4">Upper left corner: x = ' + fileStruct.SVG.rectListObj[i].x;
                                    svgtable += ' y = ' + fileStruct.SVG.rectListObj[i].y;
                                    svgtable += '<br>Width = ' + fileStruct.SVG.rectListObj[i].w;
                                    svgtable += ' Height = ' + fileStruct.SVG.rectListObj[i].h;
                                    svgtable += '</td>';
                                }
                                svgtable += '<td>' + fileStruct.SVG.rectListObj[i].numAttr + '</td>';
                                svgtable += '</tr>';
                            } //Add all circles
                            for (var i = 0; i < fileStruct.SVG.circListObj.length; i++) {
                                svgtable += '<tr>';
                                svgtable += '<td>Circle ' + (i+1) + '</td>';
                                //Check if units is empty
                                if (fileStruct.SVG.circListObj[i].units != "") {
                                    svgtable += '<td colspan="4">Centre: x = ' + fileStruct.SVG.circListObj[i].cx + fileStruct.SVG.circListObj[i].units;
                                    svgtable += ' y = ' + fileStruct.SVG.circListObj[i].cy + fileStruct.SVG.circListObj[i].units;
                                    svgtable += ' radius = ' + fileStruct.SVG.circListObj[i].r + fileStruct.SVG.circListObj[i].units;
                                    svgtable += '</td>';
                                } else {
                                    svgtable += '<td colspan="4">Centre: x = ' + fileStruct.SVG.circListObj[i].cx;
                                    svgtable += ' y = ' + fileStruct.SVG.circListObj[i].cy;
                                    svgtable += ' radius = ' + fileStruct.SVG.circListObj[i].r;
                                    svgtable += '</td>';
                                }
                                svgtable += '<td>' + fileStruct.SVG.circListObj[i].numAttr + '</td>';
                                svgtable += '</tr>';
                            } //Add all paths
                            for (var i = 0; i < fileStruct.SVG.pathListObj.length; i++) {
                                svgtable += '<tr>';
                                svgtable += '<td>Path ' + (i+1) + '</td>';
                                svgtable += '<td colspan="4">path data = ' + fileStruct.SVG.pathListObj[i].d + '</td>';
                                svgtable += '<td>' + fileStruct.SVG.pathListObj[i].numAttr + '</td>';
                                svgtable += '</tr>';
                            } //Add all groups
                            for (var i = 0; i < fileStruct.SVG.groupListObj.length; i++) {
                                svgtable += '<tr>';
                                svgtable += '<td>Group ' + (i+1) + '</td>';
                                svgtable += '<td colspan="4">' + fileStruct.SVG.groupListObj[i].children + ' child elements</td>';
                                svgtable += '<td>' + fileStruct.SVG.groupListObj[i].numAttr + '</td>';
                                svgtable += '</tr>';
                            }
                            svgtable += "</table>";
                            //Update the SVG table 
                            $('#svgview').html(svgtable);
                        },
                        error: function(error) {
                            console.log("SVG view failed to load"); //Error message for debugging
                            console.log(error);
                        }
                    });
                }
            },
            error: function(error) {
                console.log("no files received"); //Error message for console debugging
                console.log(error);
            }
        });
    });
        //console.log(selected);

    //When dropdown changes, adjust SVG view panel
    $('#svgdropdown').change(function() {
        var selected = $('#images').children("option:selected").val();
        var svgtable = "<table>";
            var svgHeader = ["Title", "Description", "Component", "Summary", "Other Attributes"];

        //console.log(selected);
        //Ajax call that loads the SVG View Panel
        $.ajax({
            type: "get",
            datatype: 'json',
            data: {
                filename: selected,
            },
            url: '/loadViewSVG',
            success: function(fileStruct) {
                console.log("SVG view successfully loaded"); //Error message for console
                //console.log(selected);
                //console.log(fileStruct.SVG.fileName);
                //console.log(fileStruct.SVG.titleDescObj);
                //console.log(fileStruct.SVG.titleDescObj.title);
                //console.log(fileStruct.SVG.titleDescObj.desc);

                //Create picture
                svgtable += '<tr><th colspan="10"><img src="' + 'uploads/' + fileStruct.SVG.fileName + '" class="svgpanelimage"></th></tr>';

                //Create first two headers
                svgtable += '<tr>';
                for (var i = 0; i < 2; i++) {
                    svgtable +='<th colspan="3">' + svgHeader[i] + '</th>';
                }
                svgtable += '</tr>';

                //Add cells for title and description
                svgtable += '<tr>';
                svgtable += '<td colspan="3">' + fileStruct.SVG.titleDescObj.title + '</td>';
                svgtable += '<td colspan="3">' + fileStruct.SVG.titleDescObj.desc + '</td>';
                svgtable += '<tr>';

                //Add last 3 headers
                svgtable += '<tr>';
                for (var i = 2; i < svgHeader.length; i++) {
                    if (i == 3) {
                        svgtable += '<th colspan="4">' + svgHeader[i] + '</th>';
                    } else {
                        svgtable += '<th>' + svgHeader[i] + '</th>';
                    }
                }
                svgtable += '</tr>';

                //Add appropriate cells for the file given
                //start with rectangles, circles, paths, then group
                for (var i = 0; i < fileStruct.SVG.rectListObj.length; i++) {
                    svgtable += '<tr>';
                    svgtable += '<td>Rectangle ' + (i+1) + '</td>';
                    if (fileStruct.SVG.rectListObj[i].units != "") {
                        svgtable += '<td colspan="4">Upper left corner: x = ' + fileStruct.SVG.rectListObj[i].x + fileStruct.SVG.rectListObj[i].units;
                        svgtable += ' y = ' + fileStruct.SVG.rectListObj[i].y + fileStruct.SVG.rectListObj[i].units;
                        svgtable += '<br>Width = ' + fileStruct.SVG.rectListObj[i].w + fileStruct.SVG.rectListObj[i].units;
                        svgtable += ' Height = ' + fileStruct.SVG.rectListObj[i].h + fileStruct.SVG.rectListObj[i].units;
                        svgtable += '</td>';
                    } else {
                        svgtable += '<td colspan="4">Upper left corner: x = ' + fileStruct.SVG.rectListObj[i].x;
                        svgtable += ' y = ' + fileStruct.SVG.rectListObj[i].y;
                        svgtable += '<br>Width = ' + fileStruct.SVG.rectListObj[i].w;
                        svgtable += ' Height = ' + fileStruct.SVG.rectListObj[i].h;
                        svgtable += '</td>';
                    }
                    svgtable += '<td>' + fileStruct.SVG.rectListObj[i].numAttr + '</td>';
                    svgtable += '</tr>';
                } //Add all circles
                for (var i = 0; i < fileStruct.SVG.circListObj.length; i++) {
                    svgtable += '<tr>';
                    svgtable += '<td>Circle ' + (i+1) + '</td>';
                    //Check if units is empty
                    if (fileStruct.SVG.circListObj[i].units != "") {
                        svgtable += '<td colspan="4">Centre: x = ' + fileStruct.SVG.circListObj[i].cx + fileStruct.SVG.circListObj[i].units;
                        svgtable += ' y = ' + fileStruct.SVG.circListObj[i].cy + fileStruct.SVG.circListObj[i].units;
                        svgtable += ' radius = ' + fileStruct.SVG.circListObj[i].r + fileStruct.SVG.circListObj[i].units;
                        svgtable += '</td>';
                    } else {
                        svgtable += '<td colspan="4">Centre: x = ' + fileStruct.SVG.circListObj[i].cx;
                        svgtable += ' y = ' + fileStruct.SVG.circListObj[i].cy;
                        svgtable += ' radius = ' + fileStruct.SVG.circListObj[i].r;
                        svgtable += '</td>';
                    }
                    svgtable += '<td>' + fileStruct.SVG.circListObj[i].numAttr + '</td>';
                    svgtable += '</tr>';
                } //Add all paths
                for (var i = 0; i < fileStruct.SVG.pathListObj.length; i++) {
                    svgtable += '<tr>';
                    svgtable += '<td>Path ' + (i+1) + '</td>';
                    svgtable += '<td colspan="4">path data = ' + fileStruct.SVG.pathListObj[i].d + '</td>';
                    svgtable += '<td>' + fileStruct.SVG.pathListObj[i].numAttr + '</td>';
                    svgtable += '</tr>';
                } //Add all groups
                for (var i = 0; i < fileStruct.SVG.groupListObj.length; i++) {
                    svgtable += '<tr>';
                    svgtable += '<td>Group ' + (i+1) + '</td>';
                    svgtable += '<td colspan="4">' + fileStruct.SVG.groupListObj[i].children + ' child elements</td>';
                    svgtable += '<td>' + fileStruct.SVG.groupListObj[i].numAttr + '</td>';
                    svgtable += '</tr>';
                }
                svgtable += "</table>";
                //Update the SVG table 
                $('#svgview').html(svgtable);
            },
            error: function(error) {
                console.log("SVG view failed to load"); //Error message for debugging
                console.log(error);
            }
        });
    });

    //Event handles an edit title press
    $('#editTitle').on("click", function(e) {
        console.log("Edit title");
    });

    //Event handles an edit description press
    $('#editDesc').on("click", function(e) {
        console.log("Edit description");
    });

    //Event handles a create SVG press
    $('#submitNewSVG').on("click", function(e) {
        console.log("Create new SVG");
    });

    //Event handles a create component press
    $('#submitComponent').on("click", function(e) {
        console.log("Create new component");
    });

    //Event handles a create shape press
    $('#reshapeComponents').on("click", function(e) {
        console.log("Reshape new component");
    });


    // Event listener form example , we can use this instead explicitly listening for events
    // No redirects if possible
    $('#someform').submit(function(e){
        $('#blah').html("Form has data: "+$('#entryBox').val());
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
        $.ajax({
            //Create an object for connecting to another waypoint
        });
    });

});