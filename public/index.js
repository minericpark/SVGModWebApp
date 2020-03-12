// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {

    var currFile = null;
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
                    currFile = selected;
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
                                }//
                                svgtable += '<td>' + fileStruct.SVG.rectListObj[i].numAttr + '<br><button id="viewAttrs" class="btn btn-outline-secondary btn-sm viewAttr" type="submit" value="' + i + '">View/Edit</button></td>';
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
                                svgtable += '<td>' + fileStruct.SVG.circListObj[i].numAttr + '<br><button id="viewAttrs" class="btn btn-outline-secondary btn-sm viewAttr" type="submit" value="' + (i + fileStruct.SVG.rectListObj.length) + '">View/Edit</button></td>';
                                svgtable += '</tr>';
                            } //Add all paths
                            for (var i = 0; i < fileStruct.SVG.pathListObj.length; i++) {
                                svgtable += '<tr>';
                                svgtable += '<td>Path ' + (i+1) + '</td>';
                                svgtable += '<td colspan="4">path data = ' + fileStruct.SVG.pathListObj[i].d + '</td>';
                                svgtable += '<td>' + fileStruct.SVG.pathListObj[i].numAttr + '<br><button id="viewAttrs" class="btn btn-outline-secondary btn-sm viewAttr" type="submit" value="' + (i + fileStruct.SVG.rectListObj.length + fileStruct.SVG.circListObj.length) + '">View/Edit</button></td>';
                                svgtable += '</tr>';
                            } //Add all groups
                            for (var i = 0; i < fileStruct.SVG.groupListObj.length; i++) {
                                svgtable += '<tr>';
                                svgtable += '<td>Group ' + (i+1) + '</td>';
                                svgtable += '<td colspan="4">' + fileStruct.SVG.groupListObj[i].children + ' child elements</td>';
                                svgtable += '<td>' + fileStruct.SVG.groupListObj[i].numAttr + '<br><button id="viewAttrs" class="btn btn-outline-secondary btn-sm viewAttr" type="submit" value="' + (i + fileStruct.SVG.rectListObj.length + fileStruct.SVG.circListObj.length + fileStruct.SVG.pathListObj.length) + '">View/Edit</button></td>';
                                svgtable += '</tr>';
                            }
                            svgtable += "</table>";
                            //Update the SVG table 
                            $('#svgview').html(svgtable);
                            $('#svgview').trigger("load", []);
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
                currFile = selected;
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
                    svgtable += '<td>' + fileStruct.SVG.rectListObj[i].numAttr + '<br><button id="viewAttrs" class="btn btn-outline-secondary btn-sm viewAttr" type="submit" value="' + i + '">View/Edit</button></td>';
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
                    svgtable += '<td>' + fileStruct.SVG.circListObj[i].numAttr + '<br><button id="viewAttrs" class="btn btn-outline-secondary btn-sm viewAttr" type="submit" value="' + (i + fileStruct.SVG.rectListObj.length) + '">View/Edit</button></td>';
                    svgtable += '</tr>';
                } //Add all paths
                for (var i = 0; i < fileStruct.SVG.pathListObj.length; i++) {
                    svgtable += '<tr>';
                    svgtable += '<td>Path ' + (i+1) + '</td>';
                    svgtable += '<td colspan="4">path data = ' + fileStruct.SVG.pathListObj[i].d + '</td>';
                    svgtable += '<td>' + fileStruct.SVG.pathListObj[i].numAttr + '<br><button id="viewAttrs" class="btn btn-outline-secondary btn-sm viewAttr" type="submit" value="' + (i + fileStruct.SVG.rectListObj.length + fileStruct.SVG.circListObj.length) + '">View/Edit</button></td>';
                    svgtable += '</tr>';
                } //Add all groups
                for (var i = 0; i < fileStruct.SVG.groupListObj.length; i++) {
                    svgtable += '<tr>';
                    svgtable += '<td>Group ' + (i+1) + '</td>';
                    svgtable += '<td colspan="4">' + fileStruct.SVG.groupListObj[i].children + ' child elements</td>';
                    svgtable += '<td>' + fileStruct.SVG.groupListObj[i].numAttr + '<br><button id="viewAttrs" class="btn btn-outline-secondary btn-sm viewAttr" type="submit" value="' + (i + fileStruct.SVG.rectListObj.length + fileStruct.SVG.circListObj.length + fileStruct.SVG.pathListObj.length) + '">View/Edit</button></td>';
                    svgtable += '</tr>';
                }
                svgtable += "</table>";
                //Update the SVG table 
                $('#svgview').html(svgtable);
                $('#svgview').trigger("load", []);
                //Hide popup if it is already visible, prevents semantic bug
                $(".popup-overlay, .popup-content").removeClass("active");
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
        console.log(currFile);
        //Open popup
        $(".popup-overlay, .popup-content").addClass("active");
    });

    //Event handles an edit description press
    $('#editDesc').on("click", function(e) {
        console.log("Edit description");
        console.log(currFile);
        //Open popup
        $(".popup-overlay, .popup-content").addClass("active");
    });

    //Event handles a create SVG press
    $('#submitNewSVG').on("click", function(e) {
        console.log("Create new SVG");
        //Open popup
        $(".popup-overlay, .popup-content").addClass("active");
    });

    //Event handles a create component press
    $('#submitComponent').on("click", function(e) {
        console.log("Create new component");
        console.log(currFile);
        //Open popup
        $(".popup-overlay, .popup-content").addClass("active");
    });

    //Event handles a create shape press
    $('#reshapeComponents').on("click", function(e) {
        console.log("Reshape new component");
        console.log(currFile);
        //Open popup
        $(".popup-overlay, .popup-content").addClass("active");
    });

    $('#svgview').on("load", function() {
        //Event handles a view attribute press
        $('.viewAttr').on("click", function(e) {
            var indexNum = $(this).val();
            console.log("view attribute");
            console.log(currFile);

            //Ajax call to retrieve attribute data from server
            $.ajax({
                type: "get",
                datatype: 'json',
                data: {
                    filename: currFile,
                    index: indexNum,
                },
                url: '/loadAttribute',
                success: function(attrStruct) {
                    console.log(attrStruct.attributes);
                    console.log("attr view successfully loaded"); //Error message for console
                    var popup = 'Viewing Attributes of Selected Component';
                    if (attrStruct.attributes.length == 0) { //If there exists no attributes
                        popup += '<table class="center">';
                        popup += '<tr><td class="noFiles">No attributes</td></tr></table>' ;
                    } else { //Otherwise, display attributes
                        popup += '<table class="center">';
                        popup += '<tr><th class="popup">Name</th><th class="popup">Value</th></tr>';
    
                        for (i = 0; i < attrStruct.attributes.length; i++) {
                            popup += '<tr><td>' + attrStruct.attributes[i].name + '</td>';
                            popup += '<td>' + attrStruct.attributes[i].value + '</td></tr>';
                        }
                        popup += '</table>';
                    }

                    popup += '<label for="editAttrName">Name</label>';
                    popup += '<input type="text" id="editAttrName"><br>';
                    popup += '<label for="editAttrValue">Value</label>';
                    popup += '<input type="text" id="editAttrValue"><br>';
                    popup += '<button id="editAttr" class="btn btn-outline-secondary btn-sm" type="submit">Edit/Add Attribute</button>';

                    $("#cus-popup-content").html(popup); //Change popup

                    //Register editAttr button click
                    $('#editAttr').on("click", function() {
                        console.log("clicked");
                        console.log(currFile);
                        console.log(indexNum);
                        var newName = $("#editAttrName").val();
                        var newValue = $("#editAttrValue").val();
                        //Check name + value
                        if (newName != "" && newValue != "") {
                            //Send ajax post ()
                            $.ajax({
                                type: "get",
                                datatype: 'json',
                                url: '/upAttribute',
                                data: {
                                    filename: currFile,
                                    index: indexNum,
                                    name: newName,
                                    value: newValue,
                                },
                                success: function(data) {
                                    console.log(data);
                                    console.log("Attr successfully edited"); //Error message for console
                                    //Print success message
                                    //Trigger the SVG viewing image to reload
                                    if (data.boolean.value == "false") {
                                        console.log("failed to change attribute for SVG");
                                        alert("SVG changes invalid, changes no saved");
                                    } else {
                                        console.log("succeed to change attribute for SVG");
                                    }
                                    //Update SVG panel
                                    $('#svgdropdown').trigger("load", []);
                                    //Hide panel
                                    $('.popup-content, .popup-layout').removeClass("active");
                                },
                                error: function(error) {
                                    //Print error message
                                }
                            });
                        } else {
                            alert("Both fields cannot be left empty");
                        }
                    });

                },
                error: function(error) {
                    console.log("attr view failed to load"); //Error message for debugging
                    console.log(error);
                }
            });
            //Utilize new parser function
            //If parser function fails, return NULL (signify fail) and send alert

            //Open popup
            $(".popup-overlay, .popup-content").addClass("active");
            
        });
    });

    //Close button for popup
    $('.close').on("click", function() {//
        $(".popup-overlay, .popup-content").removeClass("active");
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