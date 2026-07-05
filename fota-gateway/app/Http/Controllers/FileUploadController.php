<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;

class FileUploadController extends Controller
{
    public function store(Request $request) {

        // max file size: 10MB
        // todo:add bin file, elf and hex
        $request->validate([
           'bin_file' => 'required|file|max:10240',
        ]);

        $file = $request->file('bin_file');
        $filename = time().'_'.$file->getClientOriginalName();

        // store file in storage/app/bin_files directory
        $file->storeAs('bin_files', $filename, 'public');
        // dd('Upload completed');
        return redirect()->back()->with('success', 'File uploaded successfully');
    }
}
