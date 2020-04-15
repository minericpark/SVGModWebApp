// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {

    /** Base webpage front-end functionality **/

    var currFile = null;
    var currFile2 = null;
    
    //Ajax call that loads the initial SVG viewing table (calls loadTable), relies on dropdown to load
    $('#svgdropdown').on("load", function() {
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/loadTable',
            success: function(filelog) {
                console.log ("filelog: successfully loaded"); //Error message for console
                var filetable = '<table class="center">';
                var fileHeader = ["Image (click to download)", "File name (click to download)", "File size", "Number of rectangles", "Number of circles", "Number of paths", "Number of groups"];
                //If returned array length is 0, display alt message
                if (filelog.SVGLog.length == 0) {
                    console.log("filelog: no files in server");
                    filetable += '<tr>';
                    filetable += '<td class="noFiles">' + 'No files';
                    filetable += '</td></tr>';
                    
                } else { //Otherwise, run for loop that creates the table,
                    console.log("filelog: there exists files in server");
                    //Set up header of table
                    filetable += '<tr>';
                    for (var i = 0; i < fileHeader.length; i++) {
                        filetable +='<th>' + fileHeader[i] + '</th>';
                    }
                    filetable += '</tr>';
    
                    //Create cells for each files that exists within uploads
                    //console.log(filelog.SVGLog.length);
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
                        //console.log(i);
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
    });

    //Ajax call that uploads files to server (calls uploadCustom)
    var form = document.getElementById('uploadFile');
        var formData = new FormData();
        formData.append ('file', $('input[type=file]')[0].files[0]);
        
        $('#submitfile').on("click", function(e) {
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
                    //alert("Successfully submitted SVG");
                },
                error: function(error) {
                    //Print error message
                    //console.log("file failed to upload"); //Error message for console
                    //alert("Invalid SVG or file");
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
            var filedropdown2 = '<select id="images2" class="svgdropdown">';
            //If returned array length is 0, disable dropdown list
            if (filelog.SVGLog.length == 0) {
                console.log("dropdown: server is empty, no files");
                filedropdown = '<select id="images" disabled>';
                filedropdown += '<option value="No files in server">No files in server</label>';

                filedropdown2 = '<select id="images" disabled>';
                filedropdown2 += '<option value="No files in server">No files in server</label>';
            } else { //Otherwise, run for loop that creates the dropdown,
                console.log("dropdown: there exists files within the server");
                //Set up all SVG options
                //console.log(filelog.SVGLog.length);
                for (i = 0; i < filelog.SVGLog.length; i++) {
                    if (i === 0) { //Ensure first element of dropdown is selected
                        filedropdown += '<option selected value="' + filelog.SVGLog[i] + '">' + filelog.SVGLog[i] + '</label>';
                        filedropdown2 += '<option selected value="' + filelog.SVGLog[i] + '">' + filelog.SVGLog[i] + '</label>';
                    } else {
                        filedropdown += '<option value="' + filelog.SVGLog[i] + '">' + filelog.SVGLog[i] + '</label>';
                        filedropdown2 += '<option value="' + filelog.SVGLog[i] + '">' + filelog.SVGLog[i] + '</label>';
                    }
                    //Use get??
                }
            }
            filedropdown += '</select>';
            filedropdown2 += '</select>';
            $('#svgdropdown').html(filedropdown);
            $('#shapedropdown').html(filedropdown2);
            //Trigger the svg view to load
            $('#svgdropdown').trigger("load", []);
        },
        error: function(error) {
            console.log("dropdown failed to load"); //Error message for console debugging
            console.log(error);
        }
    });

    //Update dropdown when a new file enters into the actual console
    $('#newfile').on("load", function() {
        //console.log('new file!');
        
        //Ajax call that loads the dropdown menu
        $.ajax({
            type: "get",
            datatype: 'json',
            url: '/loadDropDown',
            success: function(filelog) {
                console.log("dropdown successfully re-loaded"); //Error message for console debugging
                var filedropdown = '<select id="images" class="svgdropdown">';
                var filedropdown2 = '<select id="images2" class="svgdropdown">';
                //If returned array length is 0, disable dropdown list
                if (filelog.SVGLog.length == 0) {
                    console.log("dropdown reload: server is empty");
                    filedropdown = '<select id="images" disabled>';
                    filedropdown += '<option value="No files in server">No files in server</label>';
                    
                    filedropdown2 = '<select id="images" disabled>';
                    filedropdown2 += '<option value="No files in server">No files in server</label>';
                } else { //Otherwise, run for loop that creates the dropdown,
                    console.log("dropdown reload: there exists files within server");
                    //Set up all SVG options
                    //console.log(filelog.SVGLog.length);
                    for (i = 0; i < filelog.SVGLog.length; i++) {
                        if (i === 0) { //Ensure first element of dropdown is selected
                            filedropdown += '<option selected value="' + filelog.SVGLog[i] + '">' + filelog.SVGLog[i] + '</label>';
                            filedropdown2 += '<option selected value="' + filelog.SVGLog[i] + '">' + filelog.SVGLog[i] + '</label>';
                        } else {
                            filedropdown += '<option value="' + filelog.SVGLog[i] + '">' + filelog.SVGLog[i] + '</label>';
                            filedropdown2 += '<option value="' + filelog.SVGLog[i] + '">' + filelog.SVGLog[i] + '</label>';
                        }
                        //Use get??
                    }
                }
                filedropdown += '</select>';
                filedropdown2 += '</select>';
                $('#svgdropdown').html(filedropdown);
                $('#shapedropdown').html(filedropdown2);
                //Trigger the svg view to load
                $('#svgdropdown').trigger("load", []);
            },
            error: function(error) {
                console.log("dropdown failed to load"); //Error message for console debugging
                console.log(error);
            }
        });
    });

    //Loads svg table when dropdown loads
    $('#svgdropdown').on("load", function() {
        var detectFiles = null;
        var selected = null;
        var selected2 = null;
        var svgtable = '<table class="svgtablecss">';
            var svgHeader = ["Title", "Description", "Component", "Summary", "Other Attributes"];

        $.ajax({
            type: "get",
            datatype: 'json',
            url: '/loadDropDown',
            success: function(filelog) {
                console.log("svgpanel: server checked");
                detectFiles = filelog.SVGLog.length;
                if (detectFiles == 0) { //File is empty, do not ajax call, just set svgtable to empty
                    console.log("svgpanel: server is empty");
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
                    console.log('svgpanel: server not empty');
                    selected = $('#images').children("option:selected").val();
                    selected2 = $('#images2').children("option:selected").val();
                    currFile = selected;
                    currFile2 = selected2;
                    $.ajax({
                        type: "get",
                        datatype: 'json',
                        data: {
                            filename: selected,
                        },
                        url: '/loadViewSVG',
                        success: function(fileStruct) {
                            console.log("svgpanel: successfully loaded with given file"); //Error message for console
                            //console.log(selected);
                            //console.log(fileStruct.SVG.fileName);
                            //console.log(fileStruct.SVG.titleDescObj);
                            //console.log(fileStruct.SVG.titleDescObj.title);
                            //console.log(fileStruct.SVG.titleDescObj.desc);
                            //Reset SVG
                            $('#svgview').html("");
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
                                    svgtable += ' cy = ' + fileStruct.SVG.circListObj[i].cy + fileStruct.SVG.circListObj[i].units;
                                    svgtable += ' radius = ' + fileStruct.SVG.circListObj[i].r + fileStruct.SVG.circListObj[i].units;
                                    svgtable += '</td>';
                                } else {
                                    svgtable += '<td colspan="4">Centre: x = ' + fileStruct.SVG.circListObj[i].cx;
                                    svgtable += ' cy = ' + fileStruct.SVG.circListObj[i].cy;
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
        var selected2 = $('#images2').children("option:selected").val();
        var svgtable = '<table class="svgtablecss">';
            var svgHeader = ["Title", "Description", "Component", "Summary", "Other Attributes"];

        console.log("dropdown: selected an element");
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
                currFile2 = selected2;
                console.log("svgpanel: changed successfully"); //Error message for console
                
                //Clear SVG view
                $('#svgview').html("");
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
                //Clear popup
                $('#cus-popup-content').html("");
            },
            error: function(error) {
                console.log("SVG view failed to load"); //Error message for debugging
                console.log(error);
            }
        });
    });

    //Event handles an edit title press
    $('#editTitle').on("click", function(e) {
        $('.popup-content, .popup-overlay').removeClass("active");
        //Clear panel
        $('#cus-popup-content').html("");
        //console.log("Edit title");
        console.log('edit title: pressed');

        var titlePanel = 'Edit Popup<br>';
        titlePanel += '<label for="editTitleSub">Title</label>';
        titlePanel += '<input type="text" id="editTitleSub"><br>';
        titlePanel += '<button id="submitTitle" class="btn btn-outline-secondary btn-sm" type="submit">Submit</button>';
        
        $('#cus-popup-content').html(titlePanel);

        //Register submitDescription button click
        $('#submitTitle').on("click", function() {
            console.log('edit title: submit pressed');
            //console.log(currFile);
            var newTitle = $("#editTitleSub").val();
            //console.log(newTitle);
            //Assure that null/empty is covered
            if (newTitle == "") {
                newTitle = " ";
            }
            //Check name + value
            $.ajax({
                type: "get",
                datatype: 'json',
                url: '/upTitle',
                data: {
                    filename: currFile,
                    title: newTitle,
                },
                success: function(data) {
                    //console.log(data);
                    console.log("edit title: successfully edited"); //Error message for console
                    //Print success message
                    //Trigger the SVG viewing image to reload
                    if (data.boolean.value == "false") {
                        console.log("ERROR: failed to change title for SVG");
                        alert("SVG changes invalid, changes no saved");
                    } else {
                        //console.log("succeed to change title for SVG");
                    }
                    //Update SVG panel
                    $('#svgdropdown').trigger("load", []);
                    //Hide panel
                    $('.popup-content, .popup-overlay').removeClass("active");
                    //Clear panel
                    $('#cus-popup-content').html("");
                },
                error: function(error) {
                    //Print error message
                }
            });
        });

        //Open popup if currfile not null
        if (currFile != null) {
            $(".popup-overlay, .popup-content").addClass("active");
        }
    });

    //Event handles an edit description press
    $('#editDesc').on("click", function(e) {
        //Hide panel
        $('.popup-content, .popup-overlay').removeClass("active");
        //Clear panel
        $('#cus-popup-content').html("");
        console.log('edit description: pressed');
        //console.log(currFile);
        var descPanel = 'Edit Popup<br>';
        descPanel += '<label for="editDesc">Description</label>';
        descPanel += '<input type="text" id="editDescSub"><br>';
        descPanel += '<button id="submitDesc" class="btn btn-outline-secondary btn-sm" type="submit">Submit</button>';
        $('#cus-popup-content').html(descPanel);

        //Register submitDescription button click
        $('#submitDesc').on("click", function() {
            console.log('edit description: submit pressed');
            //console.log(currFile);
            var newDesc = $("#editDescSub").val();
            //Cover empty string case
            if (newDesc == "") {
                newDesc = " ";
            }
            //Check name + value
            $.ajax({
                type: "get",
                datatype: 'json',
                url: '/upDesc',
                data: {
                    filename: currFile,
                    desc: newDesc,
                },
                success: function(data) {
                    //console.log(data);
                    console.log("edit description: successfully edited"); //Error message for console
                    //Print success message
                    //Trigger the SVG viewing image to reload
                    if (data.boolean.value == "false") {
                        console.log("ERROR: failed to change desc for SVG");
                        alert("SVG changes invalid, changes no saved");
                    } else {
                        //console.log("succeed to change desc for SVG");
                    }
                    //Update SVG panel
                    $('#svgdropdown').trigger("load", []);
                    //Hide panel
                    $('.popup-content, .popup-overlay').removeClass("active");
                    //Clear panel
                    $('#cus-popup-content').html("");
                },
                error: function(error) {
                    //Print error message
                }
            });
        });

        //Open popup
        if (currFile != null) {
            $(".popup-overlay, .popup-content").addClass("active");
        }
    });

    //Event handles a create SVG press
    $('#submitNewSVG').on("click", function(e) {
        console.log('new svg: pressed');
       //Hide panel
       $('.popup-content-mini, .popup-overlay-mini').removeClass("active");
       //Clear panel
       $('#cus-popup-content-mini').html("");

       var createSVGPanel = 'New SVG Popup<br>';
       createSVGPanel += '<label for="newSVGSub">Filename</label>';
       createSVGPanel += '<input type="text" id="newSVGSub"><br>';
       createSVGPanel += '<button id="submitNewP2" class="btn btn-outline-secondary btn-sm" type="submit">Submit</button>';
       $('#cus-popup-content-mini').html(createSVGPanel);

       //Register submitDescription button click
       $('#submitNewP2').on("click", function() {
           console.log("newsvg: submit pressed");
           //console.log(currFile);
           var newSVG = $("#newSVGSub").val();

           //Check name + value
           $.ajax({
               type: "get",
               datatype: 'json',
               url: '/checkNewSVG',
               data: {
                   filename: newSVG,
               },
               success: function(data) {
                   //console.log(data);
                   //Print success message
                   //Trigger the SVG viewing image to reload
                   if (data.boolean == false) {
                       console.log("ERROR: file exists within server");
                       alert("SVG with filename " + newSVG + " is either invalid or already exists, please enter a different filename");
                   } else {
                       console.log("newsvg: file does not exist within server");
                       //Call ajax command to add it
                       $.ajax({
                            type: "get",
                            datatype: 'json',
                            url: '/createNewSVG',
                            data: {
                                filename: newSVG,
                            },
                            success: function(valid) {
                                //console.log(valid);
                                console.log("newsvg: successfully sent"); //Error message for console
                                //Print success message
                                if (valid.boolean.value == false) {
                                    console.log("ERROR: failed to add new SVG");
                                    alert("SVG file invalid, file not saved");
                                } else {
                                    //console.log("succeed to add new SVG");
                                }
                            },
                            error: function(error) {
                                console.log(error);
                            }
                       });
                   }
                   //Update dropdown
                   $('#newfile').trigger("load", []);
                   //Hide panel
                   $('.popup-content-mini, .popup-overlay-mini').removeClass("active");
                   //Clear panel
                   $('#cus-popup-content-mini').html("");
               },
               error: function(error) {
                   //Print error message
                   console.log(error);
               }
           });
       });

       //console.log("Add new SVG");
    
       //Open popup
       $(".popup-overlay-mini, .popup-content-mini").addClass("active");
    });

    //Event handles a create component press
    $('#submitComponent').on("click", function(e) {
        //Hide panel
        $('.popup-content, .popup-overlay').removeClass("active");
        //Clear panel
        $('#cus-popup-content').html("");
        console.log("new shape: pressed");
        //console.log(currFile);

        var totalPanel = 'New Shape Popup<br>';
        var addMenu;

        var shapeDropDown = '<select id="shapeSelec">';
        shapeDropDown += '<option selected value="rect">Rectangle</label>';
        shapeDropDown += '<option value="circ">Circle</label></select>';

        addMenu = '<div id="shapeMen">Select a shape to get started';
        addMenu += '</div>';

        //If dropdown changes, change panel
        totalPanel += shapeDropDown;
        totalPanel += addMenu;

        $('#cus-popup-content').html(totalPanel);

        //Register event changes when dropdown item changes
        $('#shapeSelec').change(function() {
            var selected = $('#shapeSelec').children("option:selected").val();
            console.log('new shape: select changed');

            //Change menu to rectangle
            if (selected === "rect") {
                //console.log('changed to rect');
                addMenu = "";

                //Create new menu
                addMenu = '<br><label for="newRectX">x</label>';
                addMenu += '<input type="text" id="newRectX"><br>';
                addMenu += '<label for="newRectY">y</label>';
                addMenu += '<input type="text" id="newRectY"><br>';
                addMenu += '<label for="newRectH">height</label>';
                addMenu += '<input type="text" id="newRectH"><br>';
                addMenu += '<label for="newRectW">width</label>';
                addMenu += '<input type="text" id="newRectW"><br>';
                addMenu += '<label for="newRectU">units</label>';
                addMenu += '<input type="text" id="newRectU"><br>';
                addMenu += '<br><button id="submitNewShape" class="btn btn-outline-secondary btn-sm" type="submit">Submit</button>';

                $('#shapeMen').html(addMenu);
                //If submit pressed, register new rectangle
                $('#submitNewShape').on("click", function(e) {
                    console.log('new shape: pressed submit');
                    var newX = $("#newRectX").val();
                    var newY = $("#newRectY").val();
                    var newH = $("#newRectH").val();
                    var newW = $("#newRectW").val();
                    var newU = $("#newRectU").val();
                    //Assure all field values are not empty and they are all numbers
                    if (newX != "" && newY != "" && newH != "" && newW != "" && !isNaN(newX) && !isNaN(newY) && !isNaN(newH) && !isNaN(newW)) {
                        if (newH < 0 || newW < 0) {
                            alert ('Cannot set width and height below 0, please try again');
                            console.log('ERROR: width and height invalid');
                        } else {
                            //all clear
                            if (newU == "") {
                                newU = " ";
                            }//

                            var newObj = {
                                x: newX,
                                y: newY,
                                w: newW,
                                h: newH,
                                units: newU,
                            };
                            $.ajax({
                                type: "get",
                                datatype: 'json',
                                url: '/addNewRect',
                                data: {
                                    filename: currFile2,
                                    jsonstring: newObj,
                                },
                                success: function(valid) {
                                    //console.log(valid);
                                    console.log("newshape: created and added"); //Error message for console
                                    //Print success message
                                    if (valid.boolean.value == false) {
                                        console.log("ERROR: failed to add new shape");
                                        alert("SVG file invalid, file not saved");
                                    } else {
                                        //console.log("succeed to add new shape");
                                    }
                                    location.reload(true);
                                },
                                error: function(error) {
                                    console.log(error);
                                }
                           });
                        }
                    } else {
                        alert('Not all required values filled or invalid entry, please try again');  
                        console.log('ERROR: values empty, failed to add');                      
                    }
                    //Hide panel
                   $('.popup-content, .popup-overlay').removeClass("active");
                   //Clear panel
                   $('#cus-popup-content').html("");
                });
            } else {
                //console.log('changed to circ');
                addMenu = "";
                
                //Create new menu
                addMenu = '<br><label for="newCircX">x</label>';
                addMenu += '<input type="text" id="newCircX"><br>';
                addMenu += '<label for="newCircY">y</label>';
                addMenu += '<input type="text" id="newCircY"><br>';
                addMenu += '<label for="newCircR">radius</label>';
                addMenu += '<input type="text" id="newCircR"><br>';
                addMenu += '<label for="newCircU">units</label>';
                addMenu += '<input type="text" id="newCircU"><br>';
                addMenu += '<br><button id="submitNewShape" class="btn btn-outline-secondary btn-sm" type="submit">Submit</button>';

                $('#shapeMen').html(addMenu);
                //If submit pressed, register new circle
                $('#submitNewShape').on("click", function(e) {
                    console.log('new shape: pressed submit');
                    var newX = $("#newCircX").val();
                    var newY = $("#newCircY").val();
                    var newR = $("#newCircR").val();
                    var newU = $("#newCircU").val();
                    //Assure all field values are not empty
                    if (newX != "" && newY != "" && newR != "" && !isNaN(newX) && !isNaN(newY) && !isNaN(newR)) {
                        if (newR < 0) {
                            alert ('Cannot set radius below 0, please try again');
                            console.log('ERROR: r value invalid, failed to add');
                        } else {
                            //all clear
                            if (newU == "") {
                                newU = " ";
                            }

                            var newObj = {
                                cx: newX,
                                cy: newY,
                                r: newR,
                                units: newU,
                            };
                            $.ajax({
                                type: "get",
                                datatype: 'json',
                                url: '/addNewCirc',
                                data: {
                                    filename: currFile2,
                                    jsonstring: newObj,
                                },
                                success: function(valid) {
                                    //console.log(valid);
                                    console.log("new shape: created and added"); //Error message for console
                                    //Print success message
                                    if (valid.boolean.value == false) {
                                        console.log("ERROR: failed to add new shape");
                                        alert("SVG file invalided, file not saved");
                                    } else {
                                        //console.log("succeed to add new shape");
                                    }
                                    location.reload(true);
                                },
                                error: function(error) {
                                    console.log(error);
                                }
                           });
                        }
                    } else {
                        alert('Not all required values filled or invalid entry, please try again');   
                        console.log('ERROR: some fields empty');                     
                    }
                    //Hide panel
                   $('.popup-content, .popup-overlay').removeClass("active");
                   //Clear panel
                   $('#cus-popup-content').html("");
                });
            }
            //Change html panel 
        });

        //Open popup
        if (currFile2 != null) {
            $(".popup-overlay, .popup-content").addClass("active");
        }
    });

    //Event handles a reshape shapes press
    $('#reshapeComponents').on("click", function(e) {
        //Hide panel
        $('.popup-content, .popup-overlay').removeClass("active");
        //Clear panel
        $('#cus-popup-content').html("");
        console.log("scale shape: pressed");
        //console.log(currFile);

        var totalPanel = 'All Shape Resize Popup<br>';

        var shapeDropDown = '<select id="shapeSelec">';
        var shapeScalePan = '<br><label for="newShapeScale">Scaling Factor</label>';
        shapeDropDown += '<option selected value="rect">Rectangle</label>';
        shapeDropDown += '<option value="circ">Circle</label></select>';

        shapeScalePan += '<input type="text" id="newShapeScale"><br>';
        shapeScalePan += '<button id="submitScale" class="btn btn-outline-secondary btn-sm" type="submit">Scale</button>';

        totalPanel += shapeDropDown;
        totalPanel += shapeScalePan;

        $('#cus-popup-content').html(totalPanel);

        //Register shape scale
        $('#submitScale').on("click", function(e) {
            var selected = $('#shapeSelec').children("option:selected").val();
            var tmpScale = $('#newShapeScale').val();
            console.log('scaleshape: submit pressed');
            //console.log(selected);
            //Determine if values are valid or not
            if (!isNaN(tmpScale) && tmpScale >= 0 && tmpScale != "") { //
                if (selected == 'circ') {
                    //console.log ('circle selected');
                    $.ajax({
                        type: "get",
                        datatype: 'json',
                        url: '/scaleAllCircs',
                        data: {
                            filename: currFile2,
                            scaleFac: tmpScale,
                        },
                        success: function(valid) {
                            //console.log(valid);
                            //console.log("New shape created"); //Error message for console
                            //Print success message
                            if (valid.boolean.value == false) {
                                console.log("ERROR: failed to scale shapes");
                                alert("SVG file invalid, file not saved");
                            } else {
                                console.log("scaleshape: succeed in scaling");
                            }
                            location.reload(true);
                        },
                        error: function(error) {
                            console.log(error);
                        }
                   });
                } else {
                    console.log ('rectangle selected');
                    $.ajax({
                        type: "get",
                        datatype: 'json',
                        url: '/scaleAllRects',
                        data: {
                            filename: currFile2,
                            scaleFac: tmpScale,
                        },
                        success: function(valid) {
                            //console.log(valid);
                            //console.log("New shape created"); //Error message for console
                            //Print success message
                            if (valid.boolean.value == false) {
                                console.log("ERROR: failed to scale shapes");
                                alert("SVG file invalided, file not saved");
                            } else {
                                console.log("scaleshape: success scaling shapes");
                            }
                            location.reload(true);
                        },
                        error: function(error) {
                            console.log(error);
                        }
                   });
                }
            } else {
                alert ('Given scaling factor is not a number or invalid. Please try again');
                console.log('ERROR: Scaling factor invalid');
            }
            
        });

        //Open popup
        if (currFile != null) {
            $(".popup-overlay, .popup-content").addClass("active");
        }
    });

    //Creates all buttons for 'view/edit elements/
    $('#svgview').on("load", function() {
        console.log('svgpanel: loaded');
        //Event handles a view attribute press
        $('.viewAttr').on("click", function(e) {
            var indexNum = $(this).val();
            console.log("svgpanel: view attribute pressed");
            //console.log(currFile); 
            //Hide panel
            $('.popup-content, .popup-overlay').removeClass("active");
            //Clear panel
            $('#cus-popup-content').html("");

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
                    //console.log(attrStruct.attributes);
                    console.log("svgpanel: attr view successfully loaded"); //Error message for console
                    var popup = 'Viewing Attributes of Selected Component';
                    if (attrStruct.attributes.length == 0) { //If there exists no attributes
                        popup += '<br><table class="center"><tr><td>Table of Contents For Modification: <br>Rectangle x = x, Rectangle y = y, Rectangle height = height, Rectangle width = width<br>Circle x = cx, Circle y = cy, Circle radius = r<br>Data = d<br>Units = units</td></tr></table>';
                        popup += '<table class="center">';
                        popup += '<tr><td class="noFiles">No attributes</td></tr></table>' ;
                    } else { //Otherwise, display attributes
                        popup += '<br><table class="center"><tr><td>Table of Contents For Modification: <br>Rectangle x = x, Rectangle y = y, Rectangle height = height, Rectangle width = width<br>Circle x = cx, Circle y = cy, Circle radius = r<br>Data = d<br>Units = units</td></tr></table>';
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
                        console.log("svgpanel: edit attr clicked");
                        //console.log(currFile);
                        //console.log(indexNum);
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
                                    //console.log(data);
                                    //Print success message
                                    //Trigger the SVG viewing image to reload
                                    if (data.boolean.value == "false") {
                                        console.log("ERROR: failed to change attribute for SVG");
                                        alert("SVG changes invalid, changes no saved");
                                    } else {
                                        console.log("svgpanel: succeed to change attribute for SVG");
                                    }
                                    //Update SVG panel
                                    $('#svgdropdown').trigger("load", []);
                                    //Update SVG panel
                                    $('#svgview').trigger("load", []);
                                    //Refresh the page after attribute changes
                                    location.reload(true);
                                    //Hide panel
                                    $('.popup-content, .popup-overlay').removeClass("active");
                                    //Clear panel
                                    $('#cus-popup-content').html("");
                                },
                                error: function(error) {
                                    //Print error message
                                }
                            });
                        } else {
                            alert("Both fields cannot be left empty");
                            console.log('ERROR: Fields left empty');
                        }
                    });

                },
                error: function(error) {
                    //console.log("attr view failed to load"); //Error message for debugging
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
        console.log('pressed close on popup');
        $(".popup-overlay, .popup-content").removeClass("active");
        //Clear popup content
        $('#cus-popup-content').html("");
    });


    /********************** */
    $('#shapedropdown').change(function() {
        var selected = $('#images2').children("option:selected").val();
        console.log('new shape: select changed');
        currFile2 = selected;
    });

    //Event handles an edit svg attribute press
    $('#viewEditSVG').on("click", function(e) {
        //Hide panel
        $('.popup-content, .popup-overlay').removeClass("active");
        //Clear panel
        $('#cus-popup-content').html("");
        console.log('view/edit svg attributes: pressed');
        //console.log(currFile);

        //Ajax call to retrieve attribute data from server
        $.ajax({
            type: "get",
            datatype: 'json',
            data: {
                filename: currFile,
            },
            url: '/loadAttributeSVG',
            success: function(attrStruct) {
                //console.log(attrStruct.attributes);
                console.log("view/edit svg attribute: successfully loaded"); //Error message for console
                var popup = 'Viewing Attributes of SVG';
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
                    console.log("svgpanel: edit attr clicked");
                    //console.log(currFile);
                    //console.log(indexNum);
                    var newName = $("#editAttrName").val();
                    var newValue = $("#editAttrValue").val();
                    //Check name + value
                    if (newName != "" && newValue != "") {
                        //Send ajax post ()
                        $.ajax({
                            type: "get",
                            datatype: 'json',
                            url: '/upSVGAttribute',
                            data: {
                                filename: currFile,
                                name: newName,
                                value: newValue,
                            },
                            success: function(data) {
                                //console.log(data);
                                //Print success message
                                //Trigger the SVG viewing image to reload
                                if (data.boolean.value == "false") {
                                    console.log("ERROR: failed to change attribute for SVG");
                                    alert("SVG changes invalid, changes no saved");
                                } else {
                                    console.log("svgpanel: succeed to change attribute for SVG");
                                }
                                //Update SVG panel
                                $('#svgdropdown').trigger("load", []);
                                //Update SVG panel
                                $('#svgview').trigger("load", []);
                                //Refresh the page after attribute changes
                                location.reload(true);
                                //Hide panel
                                $('.popup-content, .popup-overlay').removeClass("active");
                                //Clear panel
                                $('#cus-popup-content').html("");
                            },
                            error: function(error) {
                                //Print error message
                            }
                        });
                    } else {
                        alert("Both fields cannot be left empty");
                        console.log('ERROR: Fields left empty');
                    }
                });

            },
            error: function(error) {
                //console.log("attr view failed to load"); //Error message for debugging
                console.log(error);
            }
        });

        //Open popup
        if (currFile != null) {
            $(".popup-overlay, .popup-content").addClass("active");
        }
    });

    /** Database front-end and UI from this point **/

});