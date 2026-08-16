<?php

use Illuminate\Support\Facades\Route;
use App\Http\Controllers\FileUploadController;

// home page route
Route::get('/', function () {
    return view('home');
});

// Route to upload bin file
Route::post('/upload_bin',
    [FileUploadController::class, 'store'])
    ->name('upload.store');

//Route::get('/files/{document}/download', [FileUrlController::class, 'signedDownload'])
//    ->name('files.download')
//    ->middleware('signed'); // Validates the signature

// This will serve the binary file to the GET request
Route::get('/storage/bin_files/{filename}', function($filename) {
    $path = storage_path('app/public/' . $filename);

    // check if file exists
    if(!file_exists($path)) {
        return response()->json([
            'error' => 'Firmware file not found'
        ], 404);
    }

    return response()->file($path, 
        ['Content-Type' => 'application/octet-stream']);

});

// returns the metadata for the firmware file
Route::get('/updates/metadata',  [FileUploadController::class, 'expose_metadata']);
