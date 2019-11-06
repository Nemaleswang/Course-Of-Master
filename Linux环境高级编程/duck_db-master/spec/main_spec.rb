describe 'database' do
  before do
    `rm -rf ./data/db.bin`
  end

  def run_script(commands)
    raw_output = nil
    IO.popen("./duck_db", "r+") do |pipe|
      commands.each do |command|
        begin
          pipe.puts command
        rescue Errno::EPIPE
          break
        end
      end

      pipe.close_write

      # Read entire output
      raw_output = pipe.gets(nil)
    end
    raw_output.split("\n")
  end


  it 'insert test' do
    script = (1..200).map do |i|
      "insert db #{i} #{i} #{18} class#{i}"
    end
    script << ".exit"
    result = run_script(script)
    expect(result.last(1)).to match_array([
      ">> bye!",
    ])
  end
end
