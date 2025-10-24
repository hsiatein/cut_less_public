using CSV
using DataFrames
using Plots
using StatsBase
using Statistics   # ✅ 用于 var()
using Distributions


# name = "input7"
df = CSV.read("$(name).csv", DataFrame)

cols = ["TotalVolume", "UtilVolume", "UtilRate", "PartsNum", "SheetsNum", "CutsNum"]

plot(title = "Normal Distributions with μ=1 and different cv",
     xlabel = "x", ylabel = "Density", legend = :top)

println("$(name)各字段变异系数：")
x = 0:0.001:2

for (i, col) in enumerate(cols)
    if(col ∉ ["TotalVolume","CutsNum","PartsNum"])
        continue
    end
    data = df[!, col]
    cv = std(data) / mean(data)
    println(rpad(col, 12), " = ", cv)

    dist = Normal(1, cv)
    y=pdf.(dist, x)
    plot!(x, y, label = "$(col) = $(cv)")

    mask = (x .>= 0.95) .& (x .<= 1.05)
    x_fill = x[mask]
    y_fill = y[mask]

    plot!(x_fill, y_fill, fillrange = 0, fillalpha = 0.3,
          label = false)
end

savefig("$(name).png")
println("✅ 已保存为 $(name).png")


# include("plot_seed.jl")
