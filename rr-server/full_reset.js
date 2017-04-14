var fs = require("fs");
var readline = require("readline");
var exec = require('child_process').exec;

var captionedPath = __dirname + "/public/imgs/captioned/";
var uploadedPath = __dirname + "/public/imgs/uploaded/";
var conversionPath = __dirname + "/public/imgs/conversion/";

var rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

function input(prompt, callback) {
    rl.question(prompt, function(x) {
        rl.close();
        callback(x);
    });
}

function resetJsonFile(jsonFile) {
    fs.writeFile(jsonFile, "{}", function(err) {
        if (err) {
            return console.error(err);
        }
    });
}

function resetStatusFile() {
    fs.writeFile(__dirname + "/status.json", '{"image_status":0}', function(err) {
        if (err) {
            return console.error(err);
        }
    });
}

function main() {
    console.log("You have chosen to reset the entire system. Answering Y/y will delete all images in the database and reset all caption data from the installation...");
    var n = input("This operation CAN'T be undone, Are you sure you want to do this? (y/n) ", function(answer) {
        if (answer == 'y' || answer == 'Y') {
            resetJsonFile(captionedPath + "vis.json");
            resetStatusFile();
            console.log("");
            console.log("FILE: vis.json, located at", captionedPath, "was RESET!");
            exec("sudo rm " + captionedPath + "*.jpg", function(error, stdout, stderr) {
                if (error !== null) {
                    console.log('exec error: ' + error);
                }
                console.log("All IMAGES have been deleted from the folder:", captionedPath);
            });
            exec("sudo rm " + uploadedPath + "*", function(error, stdout, stderr) {
                if (error !== null) {
                    console.log('exec error: ' + error);
                }
                console.log("All IMAGES have been deleted from the folder:", uploadedPath);
            });
            exec("sudo rm " + conversionPath + "*.jpg", function(error, stdout, stderr) {
                if (error !== null) {
                    console.log('exec error: ' + error);
                }
                console.log("All IMAGES have been deleted from the folder:", captionedPath);
            });
        } else if (answer == 'n' || answer == 'N') {
            console.log("");
            console.log("Nothing Was Deleted!, THANK YOU");
        } else {
            console.log("");
            console.log("ERROR. Please use either y or n to anwer.");
        }
    });
}

main()