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
            var filetable = "<table>";
            var fileHeader = ["Image (click to download)", "File name (click to download)", "File size", "Number of rectangles", "Number of circles", "Number of paths", "Number of groups"];
            //If returned array length is 0, display alt message
            if (filelog.SVGLog.length == 0) {
                console.log("empty");
                filetable += '<tr>';
                filetable += '<td>' + 'No files';
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
                    //Print success message
                    alert("Successfully submitted SVG");
                },
                error: function(error) {
                    //Print error message
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
            var filedropdown = '<select id="images">';
            //If returned array length is 0, disable dropdown list
            if (filelog.SVGLog.length == 0) {
                console.log("empty");
                filedropdown = '<select id="images" disabled>';
                
            } else { //Otherwise, run for loop that creates the dropdown,
                console.log("there exists files");
                //Set up all SVG options
                console.log(filelog.SVGLog.length);
                for (i = 0; i < filelog.SVGLog.length; i++) {
                    filedropdown += '<option value="' + filelog.SVGLog[i] + '">' + filelog.SVGLog[i] + '</label>';
                    //Use get??
                    console.log(i);
                }
            }
            filedropdown += '</select>';
            $('#svgdropdown').html(filedropdown);
        },
        error: function(error) {
            console.log(error);
        }
    });

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
                console.log(selected);
                console.log(fileStruct.SVG.fileName);
                console.log(fileStruct.SVG.titleDescObj);
                console.log(fileStruct.SVG.titleDescObj.title);
                console.log(fileStruct.SVG.titleDescObj.desc);

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

                svgtable += "</table>";
                $('#svgview').html(svgtable);
            },
            error: function(error) {
                console.log(error);
            }
        });
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