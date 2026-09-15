# 任务记录

可选记录：当用户要求记录推理画面和语言时，在本技能目录使用 `python3 scripts/record_inference.py init <会话目录> --task <任务描述>` 创建会话。每轮查看观察图像后、动作执行前，调用 `add <会话目录> --observation <本轮metadata.json> --summary-file <摘要文件>`，并通过 `--state-file` 和 `--action-file` 保存已读取的真实状态和计划动作。摘要只记录可公开的简短决策说明（观察结论、下一步动作、不确定性），不声称导出隐藏思维。保存返回的步骤编号；实际动作结束后用 `result <会话目录> <步骤编号> --result-file <真实结果文件>` 关联结果。终止时的最后一次观察也要记录，说明不再执行动作。记录失败应明确报告缺口，不得为了补记录重新执行动作。会话目录中的 `index.html` 可直接回放；详细用法见 README。

记录任务结束后（包括提前停止），若会话已有观察帧，运行 `python3 scripts/record_inference.py video <会话目录>`，直接生成 `replay.mp4` 并向用户报告绝对路径。视频为固定时长观察帧加决策摘要的回放；不得称为连续动作录像。导出失败应报告原因及仍可用的图像、JSON 和 HTML 路径。


性能：`add` 和 `result` 可加 `--defer-render`，只归档本轮资料，避免每轮重建全部 HTML。结束时 `export` 或 `video` 统一渲染。结果和图像必须保留；不要为补记录重复运动。
