<?php

use Illuminate\Support\Facades\Route;
use App\Http\Controllers\FileUploadController;

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



