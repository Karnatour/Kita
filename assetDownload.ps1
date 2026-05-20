$texturesDir = "Engine/assets/textures"
$modelsDir   = "Engine/assets/models"

# Format: "save_as_filename" = "url"
# For archives (zip/rar), use the archive name as key — it will be
# extracted into the folder and the archive deleted afterwards.
$textures = @{
  "DefaultSkybox.hdr" = "https://dl.polyhaven.org/file/ph-assets/HDRIs/hdr/4k/kloofendal_48d_partly_cloudy_puresky_4k.hdr"
  # "my_name.png"        = "https://example.com/some_texture.png"
}

$models = @{
  "main-sponza.zip"       = "https://cdrdv2.intel.com/v1/dl/getContent/830833"
}

function Download-File($url, $dest) {
  try {
    Write-Host "  Trying WebClient..."
    $wc = New-Object System.Net.WebClient
    $wc.DownloadFile($url, $dest)
  } catch {
    Write-Host "  WebClient failed, falling back to Invoke-WebRequest..."
	$ProgressPreference = 'SilentlyContinue'
    Invoke-WebRequest -Uri $url -OutFile $dest
  }
}

function Download-Assets($dir, $files) {
  foreach ($filename in $files.Keys) {
    $dest = Join-Path $dir $filename
    $ext  = [System.IO.Path]::GetExtension($filename).ToLower()

    Write-Host "Downloading $filename..."
    Download-File $files[$filename] $dest

    switch ($ext) {
      ".zip" {
        Write-Host "Extracting $filename..."
        Expand-Archive -Path $dest -DestinationPath $dir -Force
        Remove-Item $dest
        Write-Host "Extracted and removed $filename"
      }
      ".rar" {
        Write-Host "Extracting $filename..."
        # Requires WinRAR or 7-Zip installed and in PATH
        & unrar x -idq "$dest" "$dir\"
        # Or with 7-Zip: & 7z x "$dest" -o"$dir" -y | Out-Null
        Remove-Item $dest
        Write-Host "Extracted and removed $filename"
      }
      default {
        Write-Host "Saved to $dest"
      }
    }
  }
}

Download-Assets $texturesDir $textures
Download-Assets $modelsDir   $models

Write-Host "Done."