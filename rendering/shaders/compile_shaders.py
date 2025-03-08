import subprocess
import os

def compile_shader(shader_path, output_path):
    result = subprocess.run(['glslc', shader_path, '-o', output_path], capture_output=True, text=True)
    if result.returncode != 0:
        print(f"Error compiling shader {shader_path}: {result.stderr}")
    else:
        print(f"Compiled {shader_path} to {output_path}")

def find_files_with_extension(extension):
    matching_files = []
    for root, dirs, files in os.walk("."):
        for file in files:
            if file.endswith(extension):
                matching_files.append(os.path.join(root, file))
    return matching_files

def main():
    engine_bin_dir = os.getenv('ENGINE_BIN_DIR')
    if engine_bin_dir is None:
        print("ENGINE_BIN_DIR environment variable not set. Please set it to the engine's binary directory.")
        return

    vertex_shaders = find_files_with_extension('.vert')
    fragment_shaders = find_files_with_extension('.frag')

    shaders = {}
    for shader in fragment_shaders:
        shaders[shader] = shader.replace('.frag', '_frag.spv')
    for shader in vertex_shaders:
        shaders[shader] = shader.replace('.vert', '_vert.spv')

    for shader_path, output_path in shaders.items():
        compile_shader(shader_path, engine_bin_dir + output_path)


if __name__ == '__main__':
    main()